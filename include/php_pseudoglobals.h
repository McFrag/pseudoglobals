#ifndef PHP_PSEUDOGLOBALS_H
#define PHP_PSEUDOGLOBALS_H

#include "php.h"

#define PHP_PSEUDOGLOBALS_VERSION "0.1.0"

extern zend_module_entry pseudoglobals_module_entry;
#define phpext_pseudoglobals_ptr &pseudoglobals_module_entry

PHP_MINIT_FUNCTION(pseudoglobals);
PHP_MSHUTDOWN_FUNCTION(pseudoglobals);
PHP_RINIT_FUNCTION(pseudoglobals);
PHP_RSHUTDOWN_FUNCTION(pseudoglobals);
PHP_MINFO_FUNCTION(pseudoglobals);

#endif
