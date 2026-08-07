#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "../include/php_pseudoglobals.h"
#include "pseudoglobals_internal.h"

static zend_bool pseudoglobals_verify_initialized(void)
{
    zend_string *name;
    ZEND_HASH_FOREACH_STR_KEY(&PGLOB(registered), name) {
        if (name != NULL && !zend_hash_exists(&EG(symbol_table), name)) {
            php_error_docref(NULL,E_ERROR,
                "Pseudoglobal $%s was registered but was not initialized by \"%s\"",
                ZSTR_VAL(name), PGLOB(bootstrap) ? PGLOB(bootstrap) : "");
            return 0;
        }
    } ZEND_HASH_FOREACH_END();
    return 1;
}

zend_bool pseudoglobals_initialize(void)
{
    zend_file_handle file_handle;
    int result;

    if (PGLOB(initialized)) return 1;
    if (PGLOB(bootstrap) == NULL || *PGLOB(bootstrap) == '\0') {
        PGLOB(initialized) = 1;
        return 1;
    }
    if (PGLOB(initializing)) {
        php_error_docref(NULL,E_ERROR,"Recursive pseudoglobal initialization detected");
        return 0;
    }

    PGLOB(initializing) = 1;
    zend_stream_init_filename(&file_handle, PGLOB(bootstrap));
    result = zend_execute_scripts(ZEND_REQUIRE,NULL,1,&file_handle);
    PGLOB(initializing) = 0;

    if (result == FAILURE || EG(exception)) return 0;
    if (!pseudoglobals_verify_initialized()) return 0;

    PGLOB(initialized) = 1;
    return 1;
}
