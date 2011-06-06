<?php

// $Id$

/**
 * @file
 *
 * Defines a class that allows drupal forms to build via objects. Note that is still possible to build non functioning drupal forms using this
 * class, the same rules apply to FormControls and FormElements that would normally apply to a Drupal API Form.
 */
module_load_include('inc', 'objective_forms', 'FormElement');
module_load_include('inc', 'objective_forms', 'FormStorage');
module_load_include('inc', 'objective_forms', 'FormElementRegistry');

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
  public $elementRegistry;
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
  public function __construct(array $form, array &$form_state) {
    $this->form_state = &$form_state;
    $this->storage = FormStorage::initialize($form_state);
    $this->elementRegistry = FormElementRegistry::getInstance();
    $this->root = $this->createRoot($form);
  }

  /**
   *
   * @return FormElement 
   */
  protected function createRoot(array &$form = NULL) {
    return new FormElement($form);
  }

  /**
   * Seaches the form for the given form element.
   * 
   * @param hash $hash 
   * @return FormElement
   */
  public function findElement($hash) {
    return $this->root->findElement($hash);
  }

  /**
   * 
   * @param hash $hash
   * @return boolean
   */
  public function hasElement($hash) {
    return $this->findElement($hash) != NULL;
  }

  /**
   * Duplicates an element
   * 
   * @param hash $hash 
   * @return FormElement
   */
  public function duplicate($hash) {
    $element = $this->elementRegistry->get($hash);
    if ($element) {
      return clone $element;
    }
    return NULL;
  }

  /**
   * Duplicates an element from its original definition.
   * 
   * @param hash $hash
   * @return FormElement
   */
  public function duplicateOriginal($hash) {
    $original = $this->elementRegistry->getOriginal($hash);
    if ($original) {
      return clone $original;
    }
    return NULL;
  }

  /**
   * Remove the form element from this form.
   * 
   * @param hash $hash
   * @return FormElement 
   */
  public function remove($hash) {
    $element = $this->findElement($hash);
    if (isset($element)) {
      $element->orphan();
    }
    return $element;
  }

  /**
   * Validates the form.
   * 
   * @param array $form
   * @param array $form_state 
   */
  public function validate(array &$form, array &$form_state) {
    // Implemented in child classes.
  }

  /**
   * Submits the form.
   * 
   * @param array $form
   * @param array $form_state 
   */
  public function submit(array &$form, array &$form_state) {
    // Implemented in child classes.
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

  public function __toString() {
    return (string) $this->root;
  }

}