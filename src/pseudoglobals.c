#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "../include/php_pseudoglobals.h"

PHP_MINIT_FUNCTION(pseudoglobals){ return SUCCESS; }
PHP_MSHUTDOWN_FUNCTION(pseudoglobals){ return SUCCESS; }
PHP_RINIT_FUNCTION(pseudoglobals){ return SUCCESS; }
PHP_RSHUTDOWN_FUNCTION(pseudoglobals){ return SUCCESS; }

PHP_MINFO_FUNCTION(pseudoglobals)
{
    php_info_print_table_start();
    php_info_print_table_header(2,"pseudoglobals support","enabled");
    php_info_print_table_row(2,"Version",PHP_PSEUDOGLOBALS_VERSION);
    php_info_print_table_end();
}

zend_module_entry pseudoglobals_module_entry={
    STANDARD_MODULE_HEADER,
    "pseudoglobals",
    NULL,
    PHP_MINIT(pseudoglobals),
    PHP_MSHUTDOWN(pseudoglobals),
    PHP_RINIT(pseudoglobals),
    PHP_RSHUTDOWN(pseudoglobals),
    PHP_MINFO(pseudoglobals),
    PHP_PSEUDOGLOBALS_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PSEUDOGLOBALS
ZEND_GET_MODULE(pseudoglobals)
#endif
