#include "clog.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> 

#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#ifdef DEBUG
#     define mu_start() int __mu_line_number_of_first_failure = 0
#     define mu_check(condition) do{ 																					\
			if(((condition)  == false) && (__mu_line_number_of_first_failure  == 0)) 	 								\
				{__mu_line_number_of_first_failure = __LINE__;} 														\
		} while(false)
#     define mu_run(function) do{ 																						\
			int mu_line = function(); 																					\
			if(mu_line == 0) 																							\
				{fprintf(stderr ,"%sTest passed %s \n%s" , ANSI_GREEN  , #function , ANSI_RESET);} 						\
			else if (mu_line >= 1) 																						\
				{fprintf(stderr , "%sTest failed: %s at line %d \n%s" , ANSI_RED , #function , mu_line , ANSI_RESET);}	\
		} while(false)
#     define mu_end() return __mu_line_number_of_first_failure
#     define mu_check_strings_eq(s1 , s2) mu_check(strcmp((s1) , (s2)) == 0)
#endif
#endif
