#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"

#include "../include/php_pseudoglobals.h"
#include "pseudoglobals_internal.h"

ZEND_DECLARE_MODULE_GLOBALS(pseudoglobals)

static void pseudoglobals_init_globals(
    zend_pseudoglobals_globals *globals)
{
    globals->initialized = 0;
    globals->initializing = 0;
    globals->bootstrap = NULL;
    globals->register_names = NULL;
}

PHP_MINIT_FUNCTION(pseudoglobals)
{
    ZEND_INIT_MODULE_GLOBALS(
        pseudoglobals,
        pseudoglobals_init_globals,
        NULL
    );

    if (pseudoglobals_config_minit(module_number) == FAILURE) {
        return FAILURE;
    }

    if (pseudoglobals_registry_minit() == FAILURE) {
        pseudoglobals_config_mshutdown(module_number);
        return FAILURE;
    }

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pseudoglobals)
{
    pseudoglobals_registry_mshutdown();
    pseudoglobals_config_mshutdown(module_number);

    return SUCCESS;
}

PHP_RINIT_FUNCTION(pseudoglobals)
{
#if defined(COMPILE_DL_PSEUDOGLOBALS) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    PGLOB(initialized) = 0;
    PGLOB(initializing) = 0;

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(pseudoglobals)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(pseudoglobals)
{
    char registered_count[32];

    snprintf(
        registered_count,
        sizeof(registered_count),
        "%u",
        pseudoglobals_registry_count()
    );

    php_info_print_table_start();
    php_info_print_table_header(
        2,
        "pseudoglobals support",
        "enabled"
    );
    php_info_print_table_row(
        2,
        "Version",
        PHP_PSEUDOGLOBALS_VERSION
    );
    php_info_print_table_row(
        2,
        "Registered names",
        registered_count
    );
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
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PSEUDOGLOBALS
ZEND_GET_MODULE(pseudoglobals)
#endif
