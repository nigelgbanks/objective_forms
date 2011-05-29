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
   * The Drupal form state.
   * 
   * @var array
   */
  public $form_state;
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
  public $root;

  /**
   * Creates a Form.
   * 
   * @param array $form
   * @param array $form_state 
   */
  public function __construct(array &$form_state) {
    module_load_include('inc', 'objective_forms', 'FormStorage');
    module_load_include('inc', 'objective_forms', 'FormElementRegistry');
    $this->form_state = &$form;
    $this->storage = FormStorage::initialize($form_state);
    $this->registry = FormElementRegistry::getInstance();
    $this->root = $this->createRoot();
  }

  /**
   *
   * @return FormElement 
   */
  protected function createRoot() {
    return new FormElement();
  }

  /**
   * Outputs the form as an array, which can be used by the Drupal Form API.
   * 
   * @return array
   */
  public function toArray() {
    return $this->root->toArray();
  }

  /**
   * 
   * @param mixed $name
   * @return boolean
   */
  public function __isset($name) {
    return isset($this->root->$name);
  }

  /**
   *
   * @param mixed $name 
   */
  public function __unset($name) {
    unset($this->root->$name);
  }

  /**
   *
   * @param mixed $name 
   */
  public function __get($name) {
    return $this->root->$name;
  }

  /**
   *
   * @param mixed $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $this->root->$name = $value;
  }

  /**
   *
   * @param mixed $offset 
   */
  public function offsetExists($offset) {
    return $this->root->offsetExists($offset);
  }

  /**
   * 
   * @param mixed $offset 
   * @return mixed
   */
  public function offsetGet($offset) {
    return $this->root->offsetGet($offset);
  }

  /**
   * 
   * @param mixed $offset
   * @param mixed $value 
   */
  public function offsetSet($offset, $value) {
    $this->root->offsetSet($offset, $value);
  }

  /**
   * Removes the child or control identified by $offset if found.
   * 
   * @param mixed $offset 
   */
  public function offsetUnset($offset) {
    $this->root->offsetUnset($offset);
  }

}