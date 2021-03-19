/* wanda extension for PHP */

#ifndef PHP_WANDA_H
# define PHP_WANDA_H
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
extern "C"
{
#include <locale.h>
#include <php.h>
#include <php_ini.h>
#include <Zend/zend_API.h>
#include <ext/standard/info.h>
#include <zend_exceptions.h>
#include <zend_types.h>
#include <zend_compile.h>
#include <php_variables.h>

#ifdef ZTS
# include "TSRM.h"
#endif
#include <ext/standard/basic_functions.h>
#include <php_main.h>
};


#include "GlobalWanda.h"
extern zend_module_entry wanda_module_entry;
# define phpext_wanda_ptr &wanda_module_entry

# define PHP_WANDA_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_WANDA)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_WANDA_H */
