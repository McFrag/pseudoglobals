#ifndef PHP_PSEUDOGLOBALS_H
#define PHP_PSEUDOGLOBALS_H

#include "php.h"

#define PHP_PSEUDOGLOBALS_VERSION "0.1.0"

extern zend_module_entry pseudoglobals_module_entry;
#define phpext_pseudoglobals_ptr &pseudoglobals_module_entry

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

PHP_MINIT_FUNCTION(pseudoglobals);
PHP_MSHUTDOWN_FUNCTION(pseudoglobals);
PHP_RINIT_FUNCTION(pseudoglobals);
PHP_RSHUTDOWN_FUNCTION(pseudoglobals);
PHP_MINFO_FUNCTION(pseudoglobals);

#endif
