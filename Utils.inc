<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'Reflection');

/**
 * Helper methods.
 */
class ObjectiveFormsUtils {

  function __set($name, $value) {
    if (ObjectiveFormsUtils::isDrupalFormElement($value)) {
      $value = ObjectiveFormsUtils::CreateFormElementFromDrupalDefintion($value);
    }
    else if (ObjectiveFormsUtils::isDrupalFormControl($value)) {
      $value = ObjectiveFormsUtils::CreateFormControlFromDrupalDefintion($name, $value);
    }
    if (is_or_descends_from($value, 'FormElement')) {
      $this->adopt($name, $value);
    }
    else if (is_or_descends_from($value, 'FormControl')) {
      $this->setControl($value);
    }
  }

  /**
   * Converts a drupal style form element, or form control to its objective forms style.
   * 
   * @param mixed $name
   * @param mixed $value 
   * @return mixed
   *   Either a FormElement or a FormControl.
   */
  public static function convertToObjective($name, $value, array $params) {
    if (self::isObjective($name, $value)) {
      return $value;
    }
    else if (self::isDrupalFormElement($name, $value)) {
      return self::convertDrupalFormElementToObjective($value);
    }
    else if (self::isDrupalFormControl($name, $value)) {
      return self::convertDrupalFormControlToObjective($name, $value);
    }
    else {
      throw new Exception('Could not convert value to objective.');
    }
  }

  /**
   *
   * @param mixed $name
   * @param mixed $value 
   * @return boolean
   */
  private static function isObjective($name, $value) {
    return is_or_descends_from($value, 'FormElement') || is_or_descends_from($value, 'FormControl');
  }

  /**
   *
   * @param type $name
   * @param type $value
   * @return type 
   */
  private static function isDrupalFormElement($name, $value) {
    return element_child($name) && is_array($value) && !self::isDrupalFormControl($name, $value);
  }

  /**
   * It is a form control if it has name of the form / ^#.* / or if it is not an array.
   * or if it is an array that contains no form controls.
   * 
   * @param type $name
   * @param type $value
   * @return type 
   */
  private static function isDrupalFormControl($name, $value) {
    return element_property($name) || !is_array($value) || (is_array($value) && count(element_properties($value)) == 0); // Maybe name may not include the hash... 
  }

  /**
   *
   * @param array $element
   * @return FormElement 
   */
  public static function convertDrupalFormElementToObjective(array &$element) {
    $children = array();
    $controls = array();
    foreach ($element as $key => $value) {
      if (element_child($key)) {
        $control = self::convertDrupalFormElementToObjective($key, $value);
        $children[$key] = $control;
      }
      else if (element_property($key)) {
        $control = self::convertDrupalFormControlToObjective($key, $value);
        $controls[$control->name] = $control;
      }
    }
    return new FormElement($controls, $children);
  }

  /**
   *
   * @param type $name
   * @param type $value
   * @return FormControl 
   */
  public static function convertDrupalFormControlToObjective($name, $value) {
    return new FormControl($name, $value);
  }

}