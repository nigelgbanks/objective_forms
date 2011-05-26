<?php

// $Id$

/**
 * @file
 *
 * Defines a class that allows drupal forms to build via objects. Note that is still possible to build non functioning drupal forms using this
 * class, the same rules apply to FormControls and FormElements that would normally apply to a Drupal API Form.
 */

/**
 * An collection object that comprises all the FormElements in a form.
 */
class Form {

  /**
   * The Drupal Form API defined form from which this object was inialized.
   * 
   * @var array
   */
  protected $form;
  /**
   * The drupal form state.
   * 
   * @var array
   */
  protected $form_state;
  /**
   * The FormElements that make up the form.
   * 
   * @var FormElement
   */
  public $elements;

  /**
   * Creates a Form object without a Drupal Form API form.
   * 
   * @param array $form_state 
   */
  public static function createEmptyForm(array &$form_state) {
    $form = array();
    return new Form($form, $form_state);
  }

  /**
   * Creates a Form.
   * 
   * @param array $form
   * @param array $form_state 
   */
  public function __construct(array &$form, array &$form_state) {
    $this->form = $form; // Is a copy, subsequent changes to the form will not be reflected in the Form Object.
    $this->form_state = &$form;
    $this->initialize();
  }

  /**
   * Initialize the Form object.
   */
  private function initialize() {
    if (self::isValidDrupalForm($this->form)) {
      $this->initializeFromDrupalForm();
    }
    else {
      $this->initializeEmptyForm();
    }
  }

  /**
   * Initializes the Form object from a Drupal Form API form.
   */
  private function initializeFromDrupalForm() {
    // @todo Implement
  }

  /**
   * Initializes the empty form by creating the root element.
   */
  private function initializeEmptyForm() {
    $this->elements = $this->createElement();
  }

  /**
   * Meant to be overloaded by child classes so they can provide thier own implementation of FormElement.
   * @return FormElement
   */
  protected function createElement() {
    return new FormElement();
  }

  /**
   * Outputs the form as an array, which can be used by the Drupal Form API
   * 
   * @return array
   */
  public function toArray() {
    return $this->elements->toArray();
  }

}