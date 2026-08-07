#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "../include/php_pseudoglobals.h"

ZEND_DECLARE_MODULE_GLOBALS(pseudoglobals)

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY(
        "pseudoglobals.names",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        names,
        zend_pseudoglobals_globals,
        pseudoglobals_globals
    )
    STD_PHP_INI_ENTRY(
        "pseudoglobals.init_file",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        init_file,
        zend_pseudoglobals_globals,
        pseudoglobals_globals
    )
PHP_INI_END()

PHP_GINIT_FUNCTION(pseudoglobals)
{
#if defined(COMPILE_DL_PSEUDOGLOBALS) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    pseudoglobals_globals->names = NULL;
    pseudoglobals_globals->init_file = NULL;
    pseudoglobals_globals->initialized = 0;
    pseudoglobals_globals->initializing = 0;
}

PHP_MINIT_FUNCTION(pseudoglobals)
{
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pseudoglobals)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_RINIT_FUNCTION(pseudoglobals)
{
#if defined(COMPILE_DL_PSEUDOGLOBALS) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    PSEUDOGLOBALS_G(initialized) = 0;
    PSEUDOGLOBALS_G(initializing) = 0;

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(pseudoglobals)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(pseudoglobals)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "pseudoglobals support", "enabled");
    php_info_print_table_row(2, "Version", PHP_PSEUDOGLOBALS_VERSION);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

zend_module_entry pseudoglobals_module_entry = {
    STANDARD_MODULE_HEADER,
    "pseudoglobals",
    NULL,
    PHP_MINIT(pseudoglobals),
    PHP_MSHUTDOWN(pseudoglobals),
    PHP_RINIT(pseudoglobals),
    PHP_RSHUTDOWN(pseudoglobals),
    PHP_MINFO(pseudoglobals),
    PHP_PSEUDOGLOBALS_VERSION,
    PHP_MODULE_GLOBALS(pseudoglobals),
    PHP_GINIT(pseudoglobals),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_PSEUDOGLOBALS
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(pseudoglobals)
#endif
