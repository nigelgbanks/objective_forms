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
  protected $form_state;

  /**
   * Create FormPopulater.
   * 
   * @param NodeRegistry $registry 
   */
  public function __construct(FormValues $values, array &$form_state) {
    $this->values = $values;
    $this->form_state = &$form_state;
  }

  /**
   * Populate a Drupal form.
   * 
   * @param array $form
   */
  public function populate(array &$form) {
    $children = element_children($form);
    foreach ($children as $key) {
      $child = &$form[$key];
      $value = $this->getDefaultValue($child);
      if (isset($value)) {
        $this->setDefaultValue($child, $value);
      }
      $this->populate($child); // Recurse
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