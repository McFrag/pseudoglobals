#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"

#include "../include/php_pseudoglobals.h"
#include "pseudoglobals_internal.h"

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY(
        "pseudoglobals.register",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        register_names,
        zend_pseudoglobals_globals,
        pseudoglobals_globals
    )

    STD_PHP_INI_ENTRY(
        "pseudoglobals.bootstrap",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        bootstrap,
        zend_pseudoglobals_globals,
        pseudoglobals_globals
    )
PHP_INI_END()

int pseudoglobals_config_minit(int module_number)
{
    return zend_register_ini_entries(ini_entries, module_number);
}

void pseudoglobals_config_mshutdown(int module_number)
{
    zend_unregister_ini_entries(module_number);
}
