#ifndef PSEUDOGLOBALS_INTERNAL_H
#define PSEUDOGLOBALS_INTERNAL_H

#include "php.h"
#include "../include/php_pseudoglobals.h"

int pseudoglobals_config_minit(int module_number);
void pseudoglobals_config_mshutdown(int module_number);

int pseudoglobals_registry_minit(void);
void pseudoglobals_registry_mshutdown(void);
unsigned int pseudoglobals_registry_count(void);
zend_bool pseudoglobals_is_registered(zend_string *name);

zend_bool pseudoglobals_initialize(void);

#endif
