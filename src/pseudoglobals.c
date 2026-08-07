#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "../include/php_pseudoglobals.h"

ZEND_DECLARE_MODULE_GLOBALS(pseudoglobals)

static void pseudoglobals_init_globals(
    zend_pseudoglobals_globals *globals)
{
    globals->initialized = 0;
    globals->initializing = 0;
    globals->init_file = NULL;
    globals->names = NULL;
}

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY(
        "pseudoglobals.names",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        names,
        zend_pseudoglobals_globals,
        pseudoglobals_globals)

    STD_PHP_INI_ENTRY(
        "pseudoglobals.init_file",
        "",
        PHP_INI_SYSTEM,
        OnUpdateString,
        init_file,
        zend_pseudoglobals_globals,
        pseudoglobals_globals)
PHP_INI_END()

static zend_bool pseudoglobals_verify_initialized(void)
{
    zend_string *name;

    ZEND_HASH_FOREACH_STR_KEY(&PGLOB(registered), name) {
        if (name != NULL &&
            !zend_hash_exists(&EG(symbol_table), name)) {
            php_error_docref(
                NULL,
                E_ERROR,
                "Pseudoglobal $%s was registered but was not "
                "initialized by \"%s\"",
                ZSTR_VAL(name),
                PGLOB(init_file) ? PGLOB(init_file) : "");
            return 0;
        }
    } ZEND_HASH_FOREACH_END();

    return 1;
}

static zend_bool pseudoglobals_initialize(void)
{
    zend_file_handle file_handle;
    int result;

    if (PGLOB(initialized)) {
        return 1;
    }

    /*
     * An empty init_file is valid. It allows applications to register
     * pseudoglobals while initializing them themselves from PHP.
     */
    if (PGLOB(init_file) == NULL || *PGLOB(init_file) == '\0') {
        PGLOB(initialized) = 1;
        return 1;
    }

    PGLOB(initializing) = 1;

    zend_stream_init_filename(
        &file_handle,
        PGLOB(init_file));

    result = zend_execute_scripts(
        ZEND_REQUIRE,
        NULL,
        1,
        &file_handle);

    PGLOB(initializing) = 0;

    if (result == FAILURE) {
        return 0;
    }

    if (EG(exception)) {
        return 0;
    }

    if (!pseudoglobals_verify_initialized()) {
        return 0;
    }

    PGLOB(initialized) = 1;

    return 1;
}

static zend_bool pseudoglobals_callback(zend_string *name)
{
    (void) name;

    /*
     * While the bootstrap is being compiled/executed, its own references
     * to registered pseudoglobals may trigger the JIT callback again.
     * That is expected. Returning 0 here disarms the callback for that
     * auto-global without re-entering initialization.
     */
    if (PGLOB(initializing)) {
        return 0;
    }

    (void) pseudoglobals_initialize();

    /*
     * In PHP 7.4 the callback return value becomes the new "armed" state.
     * Returning 0 disarms the JIT callback after first access.
     */
    return 0;
}

static int pseudoglobals_register_name(
    const char *name,
    size_t len)
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
            name);
        return SUCCESS;
    }

    key = zend_string_init(name, len, 1);

    if (zend_hash_exists(&PGLOB(registered), key)) {
        php_error_docref(
            NULL,
            E_WARNING,
            "Ignoring duplicate pseudoglobal \"%s\"",
            ZSTR_VAL(key));
        zend_string_release(key);
        return SUCCESS;
    }

    if (zend_hash_add_empty_element(&PGLOB(registered), key) == NULL) {
        php_error_docref(
            NULL,
            E_WARNING,
            "Unable to register pseudoglobal \"%s\"",
            ZSTR_VAL(key));
        zend_string_release(key);
        return FAILURE;
    }

    if (zend_register_auto_global(
            key,
            1,
            pseudoglobals_callback) == FAILURE) {
        php_error_docref(
            NULL,
            E_WARNING,
            "Zend rejected pseudoglobal \"%s\"",
            ZSTR_VAL(key));
        zend_hash_del(&PGLOB(registered), key);
        zend_string_release(key);
        return FAILURE;
    }

    zend_string_release(key);

    return SUCCESS;
}

static int pseudoglobals_register_configured_names(void)
{
    const char *start;
    const char *p;

    if (PGLOB(names) == NULL || *PGLOB(names) == '\0') {
        return SUCCESS;
    }

    start = PGLOB(names);

    for (p = start; ; ++p) {
        if (*p == ',' || *p == '\0') {
            if (pseudoglobals_register_name(
                    start,
                    (size_t) (p - start)) == FAILURE) {
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

PHP_MINIT_FUNCTION(pseudoglobals)
{
    ZEND_INIT_MODULE_GLOBALS(
        pseudoglobals,
        pseudoglobals_init_globals,
        NULL)

    REGISTER_INI_ENTRIES();

    zend_hash_init(
        &PGLOB(registered),
        8,
        NULL,
        NULL,
        1);

    if (pseudoglobals_register_configured_names() == FAILURE) {
        return FAILURE;
    }

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(pseudoglobals)
{
    zend_hash_destroy(&PGLOB(registered));
    UNREGISTER_INI_ENTRIES();

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
        zend_hash_num_elements(&PGLOB(registered)));

    php_info_print_table_start();
    php_info_print_table_header(
        2,
        "pseudoglobals support",
        "enabled");
    php_info_print_table_row(
        2,
        "Version",
        PHP_PSEUDOGLOBALS_VERSION);
    php_info_print_table_row(
        2,
        "Registered names",
        registered_count);
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
