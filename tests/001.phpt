--TEST--
Check if wanda is loaded
--SKIPIF--
<?php
if (!extension_loaded('wanda')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "wanda" is available';
?>
--EXPECT--
The extension "wanda" is available
