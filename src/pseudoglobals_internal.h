#ifndef PSEUDOGLOBALS_INTERNAL_H
#define PSEUDOGLOBALS_INTERNAL_H

#include "php.h"
#include "php_ini.h"
#include "../include/php_pseudoglobals.h"

ZEND_BEGIN_MODULE_GLOBALS(pseudoglobals)
    zend_bool initialized;
    zend_bool initializing;
    char *bootstrap;
    char *register_names;
    HashTable registered;
ZEND_END_MODULE_GLOBALS(pseudoglobals)

ZEND_EXTERN_MODULE_GLOBALS(pseudoglobals)

#ifdef ZTS
#include "TSRM.h"
#define PGLOB(v) ZEND_MODULE_GLOBALS_ACCESSOR(pseudoglobals, v)
#else
#define PGLOB(v) (pseudoglobals_globals.v)
#endif

/* config.c */
int pseudoglobals_config_minit(void);
void pseudoglobals_config_mshutdown(void);

/* registry.c */
int pseudoglobals_registry_init(void);
void pseudoglobals_registry_shutdown(void);
int pseudoglobals_register_configured(void);
zend_bool pseudoglobals_is_registered(zend_string *name);
uint32_t pseudoglobals_registered_count(void);

/* bootstrap.c */
zend_bool pseudoglobals_initialize(void);
zend_bool pseudoglobals_auto_global_callback(zend_string *name);

#endif
