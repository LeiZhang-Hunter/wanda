/* wanda extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php_wanda.h"
/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(wanda)

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(wanda)
{
#if defined(ZTS) && defined(COMPILE_DL_WANDA)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wanda)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "wanda support", "enabled");
	php_info_print_table_end();
}
/* }}} */


/* {{{ wanda_functions[]
 */
static const zend_function_entry wanda_functions[] = {
	PHP_FE_END
};

PHP_GINIT_FUNCTION(wanda)
{
    wanda_globals->wanda = std::make_shared<Wanda::ZendWanda>();
}

//
PHP_GSHUTDOWN_FUNCTION(wanda)
{
    wanda_globals->wanda.reset();
}

PHP_MINIT_FUNCTION(wanda)
{
    WANDA_G(wanda)->executor();
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(wanda)
{
    return SUCCESS;
}

/* {{{ wanda_module_entry
 */
zend_module_entry wanda_module_entry = {
	STANDARD_MODULE_HEADER,
	"wanda",					/* Extension name */
	wanda_functions,			/* zend_function_entry */
    PHP_MINIT(wanda),
    PHP_MSHUTDOWN(wanda),							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(wanda),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(wanda),			/* PHP_MINFO - Module info */
	PHP_WANDA_VERSION,		/* Version */
    PHP_MODULE_GLOBALS(wanda),
    PHP_GINIT(wanda),
    PHP_GSHUTDOWN(wanda),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_WANDA
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(wanda)
#endif
