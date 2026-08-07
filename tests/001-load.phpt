--TEST--
pseudoglobals extension loads
--SKIPIF--
<?php if (!extension_loaded('pseudoglobals')) die('skip pseudoglobals extension not loaded'); ?>
--FILE--
<?php echo phpversion('pseudoglobals'), PHP_EOL; ?>
--EXPECT--
0.1.1
