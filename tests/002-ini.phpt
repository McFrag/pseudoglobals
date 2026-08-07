--TEST--
pseudoglobals exposes its INI settings
--SKIPIF--
<?php if (!extension_loaded('pseudoglobals')) die('skip pseudoglobals extension not loaded'); ?>
--INI--
pseudoglobals.register=_T,_CFG,_AUTH
pseudoglobals.bootstrap=/tmp/init_pseudoglobals.php
--FILE--
<?php echo ini_get('pseudoglobals.register'), PHP_EOL; echo ini_get('pseudoglobals.bootstrap'), PHP_EOL; ?>
--EXPECT--
_T,_CFG,_AUTH
/tmp/init_pseudoglobals.php
