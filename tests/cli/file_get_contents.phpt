--TEST--
Check if wanda is loaded
--SKIPIF--
<?php
if (!extension_loaded('wanda')) {
	echo 'skip';
}
var_dump(file_get_contents("http://www.baidu.com"));
?>
--FILE--
<?php
echo 'The extension "wanda" is available';
?>
--EXPECT--
The extension "wanda" is available
