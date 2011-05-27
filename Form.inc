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
class Form implements ArrayAccess {

  /**
   * The Drupal Form API defined form from which this object was inialized.
   * 
   * @var array
   */
  protected $form;
  /**
   * The Drupal form state.
   * 
   * @var array
   */
  protected $form_state;
  /**
   *
   * @var FormStorage
   */
  public $storage;
  /**
   *
   * @var FormElementRegistry
   */
  public $registry;
  /**
   * The FormElements that make up the form.
   * 
   * @var array
   */
  public $elements;

  /**
   *
   * @param array $form 
   */
  public static function isValidDrupalForm(array &$form) {
    return TRUE; // TODO implement??
  }

  /**
   * Creates a Form object without a Drupal Form API form.
   * 
   * @param array $form_state 
   */
  public static function createEmptyForm(array &$form_state) {
    $form = array();
    return new Form($form, $form_state); // In this nesscary?
  }

  /**
   * Creates a Form.
   * 
   * @param array $form
   * @param array $form_state 
   */
  public function __construct(array &$form, array &$form_state) {
    module_load_include('inc', 'objective_forms', 'FormStorage');
    module_load_include('inc', 'objective_forms', 'FormElementRegistry');
    $this->form = $form; // Is a copy, subsequent changes to the Drupal form will not be reflected in the Form.
    $this->form_state = &$form;
    $this->elements = array();
    $this->storage = FormStorage::initialize($form_state);
    $this->registry = FormElementRegistry::getInstance();
  }

  /**
   * 
   * @param mixed $name
   * @return boolean
   */
  public function __isset($name) {
    return $this->offsetExists($name);
  }

  /**
   *
   * @param mixed $name 
   */
  public function __unset($name) {
    return $this->offsetUnset($name);
  }

  /**
   *
   * @param mixed $name 
   */
  public function __get($name) {
    return $this->offsetGet($name);
  }

  /**
   *
   * @param mixed $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    return $this->offsetSet($name, $value);
  }

  /**
   *
   * @param mixed $offset 
   */
  public function offsetExists($offset) {
    return isset($this->elements[$offset]);
  }

  /**
   * 
   * @param mixed $offset 
   * @return mixed
   */
  public function offsetGet($offset) {
    if ($this->offsetExists($offset)) {
      return $this->elements[$offset];
    }
    return NULL;
  }

  /**
   * 
   * @param mixed $offset
   * @param mixed $value 
   */
  public function offsetSet($offset, $value) {
    $this->elements[$offset] = $value;
  }

  /**
   * Removes the child or control identified by $offset if found.
   * 
   * @param mixed $offset 
   */
  public function offsetUnset($offset) {
    unset($this->elements[$offset]);
  }

  /**
   * Meant to be overloaded by child classes so they can provide thier own implementation of FormElement.
   * 
   * @param $controls
   * @param $children
   * 
   * @return FormElement
   *
    protected function createElement(array $controls = NULL, array $children = NULL) {
    return new FormElement($controls, $children);
    } */

  /**
   * Outputs the form as an array, which can be used by the Drupal Form API.
   * 
   * @return array
   */
  public function toArray() {
    $output = array();
    foreach ($this->elements as $key => $element) {
      if (is_or_descends_from($element, 'FormElement')) {
        $output[$key] = $element->toArray();
      }
      else {
        $output[$key] = $element; // Property of the form.
      }
    }
    return $output;
  }

}