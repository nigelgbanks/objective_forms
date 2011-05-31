<?php

// $Id$

/**
 * @file
 *
 */

/**
 * An interface for FormProperties that want to be treated as objects.
 */
interface FormPropertyInterface {

  /**
   * Creates the FormProperty object from its drupal form property value.
   * 
   * @param mixed $value
   */
  public function __construct($value);

  /**
   * Converts this object to a drupal form property value.
   * 
   * @return mixed
   */
  public function getValue();
}