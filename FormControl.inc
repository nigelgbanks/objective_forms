<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Class repersenting a FormControls
 */
class FormControl {

  /**
   * The name of this form control.
   * 
   * @var string
   */
  protected $name;
  /**
   * The value for this form control.
   * 
   * @var mixed
   */
  public $value;

  /**
   * Checks to see if the name is a valid form control name.
   * 
   * @param string $name 
   * @return boolean
   */
  public static function validName($name) {
    return is_string($name) && strpos($name, '#') === 0;
  }

  /**
   * Create a FormControl
   * 
   * @param string $name
   * @param mixed $value 
   */
  public function __construct($name, $value) {
    if (self::validName($name)) {
      $this->name = $name;
      $this->value = $value;
    }
    else {
      throw new Exception('Try to create a FormControl with an invalid name: ' . $name);
    }
  }

}