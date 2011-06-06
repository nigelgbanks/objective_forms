<?php

// $Id$

/**
 * @file
 *
 * Associates submitted form values with FormElements.
 */
module_load_include('inc', 'objective_forms', 'FormValueTracker');

/**
 * This class stores all submitted values. It provides a mechanism for 
 * accessing submitted values via a FormElement object.
 */
class FormValues {

  /**
   * A map of submitted form values where the key is the FormElements hash.
   * 
   * @var array
   */
  protected $values;
  /**
   * A helper class that helps determine what values are associated with what elements.
   * 
   * @var FormValueTracker 
   */
  protected $tracker;

  /**
   * Checks to see if any form values were submitted to the server.
   * 
   * @param array $form_state
   * @return boolean
   */
  public static function Exists(array &$form_state) {
    return isset($form_state['values']);
  }

  /**
   * Create a FormValues object.
   * 
   * @param array $form_state
   *   Drupal Form state.
   * @param FormElement $root 
   *   The elements to associate with submitted values.
   */
  public function __construct(array &$form_state, FormElement $root) {
    $this->values = array();
    if (self::Exists($form_state)) {
      $this->tracker = new FormValueTracker($form_state['values']);
      $this->setValues($root, $this->tracker);
    }
  }

  /**
   * Store values.
   * 
   * @param FormElement $element
   *   Element to store value for,
   * @param FormValueTracker $tracker 
   */
  private function setValues(FormElement $element, FormValueTracker $tracker) {
    $this->setValue($element, $tracker);
    foreach ($element->children as $child) {
      $this->setValues($child, clone $tracker);
    }
  }

  /**
   * Associate the given FormElement, with a submitted value.
   * 
   * @param FormElement $element
   * @param FormValueTracker $tracker 
   */
  private function setValue(FormElement $element, FormValueTracker $tracker) {
    $value = $tracker->getValue($element);
    $this->values[$element->hash] = $value;
  }

  /**
   * Is there a value associated with the given form element.
   * 
   * @param hash $hash 
   * @return boolean
   */
  public function hasValue($hash) {
    return isset($this->values[$hash]);
  }

  /**
   * Get the value associated with the given FormElement.
   * 
   * @param hash $hash
   * @return mixed
   *   The submitted value for the given FormElement if found otherwise NULL.
   */
  public function getValue($hash) {
    if ($this->hasValue($hash)) {
      return $this->values[$hash];
    }
    return NULL;
  }

  /**
   * Returns all values.
   * 
   * @return array 
   */
  public function getValues() {
    return $this->values;
  }

}