/*                                                                
**  Copyright (C) 2004-2008  Smithsonian Astrophysical Observatory 
*/                                                                

/*                                                                          */
/*  This program is free software; you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation; either version 3 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/*  GNU General Public License for more details.                            */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License along */
/*  with this program; if not, write to the Free Software Foundation, Inc., */
/*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             */
/*                                                                          */

extern "C" {

#include <dslib.h>
#include <stack.h>
#include <dsnan.h>
}

#include <algorithm>
using std::find;

#include <map>
using std::map;

#include <vector>
using std::vector;


#include <cstdlib>


#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#define PAR_FILE_PAR_NAME "parameter_file_name"


int main(int argc, char** argv)
{


  char infile[DS_SZ_FNAME];
  char outfile[DS_SZ_FNAME];
  short clobber;
  short verbose;
  dsErrInitLib( dsPTGRPERR, (char*)"pars2tab" );

  if ( NULL == clinit(argv, argc, "rw" ) ) {
    err_msg("Problem opening parameter file: %s",  paramerrstr() );
    return(-1);
  }
  clgetstr( (char*)"infile", infile, DS_SZ_FNAME );
  clgetstr( (char*)"outfile", outfile, DS_SZ_FNAME );
  clobber = clgetb((char*)"clobber");
  verbose = clgeti((char*)"verbose");
  clclose();


  if ( 0 != ds_clobber( outfile, (dsErrBool)clobber, NULL)) {
       return(-2);
  }


  Stack inStack;
  if (NULL == ( inStack = stk_build(infile))) {
    err_msg("ERROR: Could not expand stack '%s'\n", infile );
    return(-1);
  } else if (( stk_count(inStack) == 0 ) ||
	     ( ( stk_count(inStack) == 1 ) && 
	       ( strlen(stk_read_num(inStack,1))==0 ))) {
    err_msg("ERROR: Empty stack '%s'\n", infile );
  }
  stk_rewind(inStack);

  char *curFile;

  vector< map<string,string> >AllValues;
  vector< string > AllPars;
  map<string,string>AllTypes;
  map<string,long>StrLens;
  map<string,string>AllPrompts;
  map<string,string>AllUnits;
  
  while ( NULL != ( curFile = stk_read_next( inStack ))) {
    paramfile paramFile;
    pmatchlist pList;
    char *curPar;

    cout << "Processing file: " << curFile << endl;

    if ( NULL == (paramFile = paramopen( curFile, NULL, 0, (char*)"rH"))) {
      err_msg("ERROR: Could not open file '%s'; skipping it\n", curFile );
      continue;
    }

    if ( NULL == ( pList = pmatchopen( paramFile, (char*)"*" ))) {
      err_msg("ERROR: Problems reading parameter file '%s'.  Error message is:",
	      curFile, paramerrstr() );
      continue;
    }

    map<string,string>ParamValues;

    

    curPar = (char*)PAR_FILE_PAR_NAME;
    char *llptr;
    if ( NULL == (llptr = strrchr( curFile, '/' ))) { /* Strip off path */
      llptr = curFile;
    } else {
      llptr++;
    }
    

    ParamValues[curPar] = string(llptr);
    AllTypes[curPar]=string("f");
    StrLens[curPar] = MAX ( StrLens[curPar], ParamValues[curPar].length());
    AllPrompts[curPar] = string("Input file name");
    AllUnits[curPar] = string("");
    if (find( AllPars.begin(), AllPars.end(), string(curPar)) 
	== AllPars.end()) {
      AllPars.push_back( string(curPar ));
    }
      


    while ( NULL != (curPar = pmatchnext( pList ) )) {
      char type[SZ_PFLINE];
      char prompt[SZ_PFLINE];
      char value[SZ_PFLINE];

      ParamInfo( paramFile, curPar, NULL, type, value, NULL, NULL, prompt );

      if ( string(curPar) == string("mode") ) continue;


      if ( AllTypes.find( curPar ) != AllTypes.end() ) {
	if ( AllTypes[curPar] != string(type) ) {
	  err_msg("WARNING: Parameter '%s' has different datatype in file '%s'. "
		  "Looking for datatype '%s', found '%s'", curPar, curFile, 
		  AllTypes[curPar].c_str(), type );
	}
      } else {
	AllTypes[curPar] = string( type );
      }
      
      ParamValues[curPar] = string(value);
      StrLens[curPar] = MAX ( StrLens[curPar], ParamValues[curPar].length());


      /* Strip the unit off the prompt.  Look for last occurance of []'s */

      char *lptr;
      char *unit;
      unit = NULL;
      if ( NULL != ( lptr = strrchr( prompt, '[' ) ) ) {
	if ( NULL != strchr( lptr , ']') ) {
	  unit = lptr+1;
	  *lptr = '\0';  /* Terminate prompt string*/
	  lptr = strchr( unit, ']' );
	  *lptr = '\0'; /* Terminate unit string */
	}
      }
      
      if ( NULL == unit ) unit=(char*)"";


      AllPrompts[curPar] = string(prompt);
      AllUnits[curPar] = string( unit );

      /* Save AllPars so that values are stored in order */
      if (find( AllPars.begin(), AllPars.end(), string(curPar)) 
	  == AllPars.end()) {
	AllPars.push_back( string(curPar ));
      }
      
    } /* end while curPar, pmatchlist */

    AllValues.push_back( ParamValues );
    ParamValues.erase( ParamValues.begin(), ParamValues.end());
    pmatchclose( pList );
    paramclose( paramFile );

  } /* end while curFile, infile stack  */
  stk_close( inStack );



  if ( 0 != ds_clobber( outfile, (dserrbool)1, NULL ) ) {
    return(-1);  /* NULL => error message will be printed */
  }

  dmBlock *outBlock;
  if ( NULL == ( outBlock = dmTableCreate( outfile ))) {
    err_msg("ERROR: Could not create output file '%s'", outfile );
    return(-1);
  }


  map<string,dmDescriptor*> AllColumns;
  long lnull = INDEFL;

  vector<string>::iterator parname;
  for (parname=AllPars.begin();parname!=AllPars.end();parname++) {

    string name = *parname;
    char type[10];

    strcpy( type, AllTypes[name].c_str() );


    switch( *type ) {
    case 'b':
      AllColumns[name] = dmColumnCreate( outBlock, (char*)name.c_str(), 
					 dmBOOL, 0, 
					 (char*)AllUnits[name].c_str(), 
					 (char*)AllPrompts[name].c_str() );
      break;
    case 'r':
      AllColumns[name] = dmColumnCreate( outBlock, (char*)name.c_str(), 
					 dmDOUBLE, 0, 
					 (char*)AllUnits[name].c_str(), 
					 (char*)AllPrompts[name].c_str() );
      break;
    case 'i':
      AllColumns[name] = dmColumnCreate( outBlock, (char*)name.c_str(), 
					 dmLONG, 0, 
					 (char*)AllUnits[name].c_str(), 
					 (char*)AllPrompts[name].c_str() );
      dmDescriptorSetNull_l(  AllColumns[name], lnull );
      break;
    case 's':
    case 'f':
    case 'p': /* 'pset' */
      AllColumns[name] = dmColumnCreate( outBlock, (char*)name.c_str(), 
					 dmTEXT, StrLens[name]+1, 
					 (char*)AllUnits[name].c_str(), 
					 (char*)AllPrompts[name].c_str() );
      break;
      
    default:
      break;
    }

  } /* end loop over parameter names */




  /* now populate data */

  vector< map<string,string> >::iterator values;
  for (values=AllValues.begin();values!=AllValues.end();values++) {
    
    for (parname=AllPars.begin();parname!=AllPars.end();parname++) {
      string name = *parname;
      
      char type[10];
      
      strcpy( type, AllTypes[name].c_str() );
      
      switch( *type ) {
      case 'b':
	unsigned char bval;
	if ( values->find(name) == values->end() ) {
	  bval = FALSE;
	} else {
	  if ( values->find(name)->second == string("yes")) 
	    bval = TRUE;
	  else
	    bval = FALSE;
	}
	dmSetScalar_q( AllColumns[name], bval );
	break;

      case 'r':
	double dval;
	if ( values->find(name) == values->end() ) {
	  ds_MAKE_DNAN(dval);
	} else  if ( values->find(name)->second == string("INDEF") ) {
	  ds_MAKE_DNAN(dval);
	} else {
	  dval = atof(values->find(name)->second.c_str()) ;
	}
	dmSetScalar_d( AllColumns[name], dval );

	break;

      case 'i':
	long lval;
	if ( values->find(name) == values->end() ) {
	  lval = INDEFL;
	} else  if ( values->find(name)->second == string("INDEF") ) {
	  lval = INDEFL;
	} else {
	  lval = atol(values->find(name)->second.c_str()) ;
	}
	dmSetScalar_l( AllColumns[name], lval );
	break;

      case 's':
      case 'f':
      case 'p':
	const char *cval;
	if ( values->find(name) == values->end() ) {
	  cval = "";
	} else {
	  cval = values->find(name)->second.c_str();
	}
	dmSetScalar_c( AllColumns[name], (char*)cval );
	break;

      default:
	break;
      } /* end type */


    } /* end over params */
    
    
    dmTableNextRow( outBlock );
    
  } /* end over values */


  dmTableClose( outBlock );

  return(0);

}
