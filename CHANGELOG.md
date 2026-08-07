# Changelog

All notable changes to this project will be documented in this file.

## [0.1.1] - 2026-08-07

### Fixed

- Moved bootstrap execution from the auto-global JIT callback to `RINIT`.
- Fixed pseudoglobal initialization under Apache when OPcache reuses cached
  op-arrays and no script compilation occurs on subsequent requests.

### Changed

- `pseudoglobals.bootstrap` is now `PHP_INI_PERDIR`, allowing different Apache
  virtual hosts or directories to use different bootstrap files.
- `pseudoglobals.register` remains `PHP_INI_SYSTEM`, because pseudoglobal names
  must be registered during module startup.

## [0.1.0] - 2026-08-07

### Added

- Initial pseudoglobal registration using `zend_register_auto_global()`.
- `pseudoglobals.register` configuration.
- PHP bootstrap initialization.
- Function-scope access without `global`.
- Support for multiple registered pseudoglobals.
- PHPT test suite.
