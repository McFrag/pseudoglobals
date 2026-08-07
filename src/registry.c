#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "../include/php_pseudoglobals.h"
#include "pseudoglobals_internal.h"

static int pseudoglobals_register_name(const char *name, size_t len)
{
    zend_string *key;

    while (len > 0 && (*name == ' ' || *name == '\t')) {
        ++name;
        --len;
    }

    while (len > 0 &&
           (name[len - 1] == ' ' || name[len - 1] == '\t')) {
        --len;
    }

    if (len == 0) {
        return SUCCESS;
    }

    if (name[0] != '_') {
        php_error_docref(
            NULL,
            E_WARNING,
            "Ignoring pseudoglobal \"%.*s\": name must begin with '_'",
            (int) len,
            name
        );
        return SUCCESS;
    }

    key = zend_string_init(name, len, 1);

    if (zend_hash_exists(&PGLOB(registered), key)) {
        php_error_docref(
            NULL,
            E_WARNING,
            "Ignoring duplicate pseudoglobal \"%s\"",
            ZSTR_VAL(key)
        );
        zend_string_release(key);
        return SUCCESS;
    }

    if (zend_hash_add_empty_element(&PGLOB(registered), key) == NULL) {
        php_error_docref(
            NULL,
            E_WARNING,
            "Unable to add pseudoglobal \"%s\" to registry",
            ZSTR_VAL(key)
        );
        zend_string_release(key);
        return FAILURE;
    }

    if (zend_register_auto_global(
            key,
            1,
            pseudoglobals_callback
        ) == FAILURE) {
        php_error_docref(
            NULL,
            E_WARNING,
            "Zend rejected pseudoglobal \"%s\"",
            ZSTR_VAL(key)
        );
        zend_hash_del(&PGLOB(registered), key);
        zend_string_release(key);
        return FAILURE;
    }

    zend_string_release(key);
    return SUCCESS;
}

static int pseudoglobals_register_configured(void)
{
    const char *start;
    const char *p;

    if (PGLOB(register_names) == NULL || *PGLOB(register_names) == '\0') {
        return SUCCESS;
    }

    start = PGLOB(register_names);

    for (p = start; ; ++p) {
        if (*p == ',' || *p == '\0') {
            if (pseudoglobals_register_name(
                    start,
                    (size_t) (p - start)
                ) == FAILURE) {
                return FAILURE;
            }

            if (*p == '\0') {
                break;
            }

            start = p + 1;
        }
    }

    return SUCCESS;
}

int pseudoglobals_registry_minit(void)
{
    zend_hash_init(
        &PGLOB(registered),
        8,
        NULL,
        NULL,
        1
    );

    if (pseudoglobals_register_configured() == FAILURE) {
        zend_hash_destroy(&PGLOB(registered));
        return FAILURE;
    }

    return SUCCESS;
}

void pseudoglobals_registry_mshutdown(void)
{
    zend_hash_destroy(&PGLOB(registered));
}

unsigned int pseudoglobals_registry_count(void)
{
    return (unsigned int) zend_hash_num_elements(&PGLOB(registered));
}

zend_bool pseudoglobals_is_registered(zend_string *name)
{
    return zend_hash_exists(&PGLOB(registered), name);
}
