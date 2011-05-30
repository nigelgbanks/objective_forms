<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'objective_forms', 'FormValues');

/**
 * Convenience class used to populate a drupal form with values submitted as post data.
 */
class FormPopulator {

  /**
   *
   * @var FormValues
   */
  protected $values;

  /**
   * Create FormPopulater.
   * 
   * @param NodeRegistry $registry 
   */
  public function __construct(FormValues $values) {
    $this->values = $values;
  }

  /**
   * Populate a Drupal form.
   * 
   * @param array $form
   */
  public function populate(array &$form) {
    foreach ($form as $element) {
      $value = $this->getDefaultValue($element);
      if (isset($value)) {
        $this->setDefaultValue($element, $value);
      }
    }
    return $form;
  }

  /**
   * Get the default value for the given element.
   * 
   * @param FormElement $element
   */
  private function getDefaultValue(array &$element) {
    if (isset($element['#hash'])) {
      return $this->getSubmittedValue($element['#hash']);
    }
    return NULL;
  }

  /**
   * Gets the submited value for the given element.
   * 
   * @param hash $hash
   * @return mixed 
   */
  private function getSubmittedValue($hash) {
    return $this->values->getValue($hash);
  }

  /**
   * Set the FormElements default value if appropriate.
   * 
   * @param FormElement $element 
   */
  private function setDefaultValue(array &$element, $value) {
    $element['#default_value'] = $value;
  }

}