<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Check to is if the object matches the given type.
 * 
 * @param object $obj
 * @param string $type 
 * 
 * @return boolean
 */
function is_type($obj, $type) {
  return isset($obj) && is_object($obj) && is_string($type) && get_class($obj) == $type;
}

/**
 * Checks if the object is a decendant of the provided type.
 * 
 * @param object $child
 * @param string $type
 * 
 * @return boolean
 */
function is_decendant($obj, $type) {
  $class = isset($obj) && is_object($obj) ? get_class($obj) : FALSE;
  $reflection = new ReflectionClass($class);
  return $reflection->isSubclassOf($type);
}