<?php

// $Id$

/**
 * @file
 *
 */

/**
 * A collection of static functions to help with processing for properties.
 */
class FormProperty {

  /**
   *
   * @staticvar type $cache
   * @return type 
   */
  public static function GetRegisteredFormPropertiesTypes() {
    static $cache;
    if (empty($cache)) {
      $cache = array();
      foreach (module_implements('objectify_properties') as $module) {
        $properties = module_invoke($module, 'objectify_properties');
        if (isset($properties) && is_array($properties)) {
          $cache = array_merge_recursive($cache, $properties);
        }
      }
    }
    return $cache;
  }

  /**
   *
   * @param type $name
   * @return type 
   */
  public static function IsRegisteredFormProperty($name) {
    $properties = self::GetRegisteredFormPropertiesTypes();
    return isset($properties[$name]);
  }

  /**
   *
   * @param type $name
   * @return type 
   */
  public static function GetProperty($name) {
    if (self::IsRegisteredFormProperty($name)) {
      $properties = self::GetRegisteredFormPropertiesTypes();
      return $properties[$name];
    }
    return NULL;
  }

  /**
   *
   * @param type $name 
   */
  public static function LoadFile($name) {
    $property = self::GetProperty($name);
    if (isset($property)) {
      module_load_include($property['type'], $property['module'], $property['name']);
    }
  }

  /**
   *
   * @param type $name
   * @param type $value
   * @return class 
   */
  public static function Create($name, $value) {
    $property = self::GetProperty($name);
    if (isset($property)) {
      $class = $property['class'];
      return new $class($value);
    }
    return NULL;
  }

  /**
   *
   * @param string $name
   * @param mixed $value 
   */
  public static function Expand($name, $value) {
    if (self::IsRegisteredFormProperty($name)) {
      self::LoadFile($name);
      return self::Create($name, $value);
    }
    return $value;
  }

}

