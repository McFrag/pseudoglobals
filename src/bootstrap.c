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
            php_error_docref(
                NULL,
                E_ERROR,
                "Pseudoglobal $%s was registered but was not initialized by \"%s\"",
                ZSTR_VAL(name),
                PGLOB(bootstrap) ? PGLOB(bootstrap) : ""
            );
            return 0;
        }
    } ZEND_HASH_FOREACH_END();

    return 1;
}

zend_bool pseudoglobals_initialize(void)
{
    zend_file_handle file_handle;
    int result;

    if (PGLOB(initialized)) {
        return 1;
    }

    if (PGLOB(bootstrap) == NULL || *PGLOB(bootstrap) == '\0') {
        PGLOB(initialized) = 1;
        return 1;
    }

    if (PGLOB(initializing)) {
        return 1;
    }

    PGLOB(initializing) = 1;

    zend_stream_init_filename(&file_handle, PGLOB(bootstrap));

    result = zend_execute_scripts(
        ZEND_REQUIRE,
        NULL,
        1,
        &file_handle
    );

    PGLOB(initializing) = 0;

    if (result == FAILURE || EG(exception)) {
        return 0;
    }

    if (!pseudoglobals_verify_initialized()) {
        return 0;
    }

    PGLOB(initialized) = 1;

    return 1;
}

zend_bool pseudoglobals_callback(zend_string *name)
{
    (void) name;

    /*
     * PHP 7.4 may invoke the JIT auto-global callback again while
     * compiling the bootstrap itself. That is expected re-entry.
     * Returning 0 disarms that particular auto-global callback.
     */
    if (PGLOB(initializing)) {
        return 0;
    }

    (void) pseudoglobals_initialize();

    /*
     * The callback return value becomes the new "armed" state.
     * 0 means no further callback for this auto-global in this request.
     */
    return 0;
}
