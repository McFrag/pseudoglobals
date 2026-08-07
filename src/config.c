#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "pseudoglobals_internal.h"

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY(
        "pseudoglobals.register",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        register_names,
        zend_pseudoglobals_globals,
        pseudoglobals_globals)

    STD_PHP_INI_ENTRY(
        "pseudoglobals.bootstrap",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        bootstrap,
        zend_pseudoglobals_globals,
        pseudoglobals_globals)
PHP_INI_END()

int pseudoglobals_config_minit(void)
{
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

void pseudoglobals_config_mshutdown(void)
{
    UNREGISTER_INI_ENTRIES();
}
