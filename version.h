#ifndef __VERSION_H_
#define __VERSION_H_

#if defined(__STDC__)
#	define C89
# 	if defined(__STDC_VERSION__)
#  		define C90
#  		if (__STDC_VERSION__ >= 199409L)
#   		define C94
#  		endif
#  		if (__STDC_VERSION__ >= 199901L)
#   		define C99
#  		endif
# 	endif
#endif

#endif // __VERSION_H_
