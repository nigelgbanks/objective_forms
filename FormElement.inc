<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'objective_forms', 'Utils');

/**
 * 
 */
class FormElement implements ArrayAccess {

  /**
   * Hash used to uniquely identify this FormElement.
   * 
   * @var hash
   */
  protected $hash;
  /**
   * The parent of this FormElement may be NULL.
   * 
   * @var FormElement
   */
  protected $parent;
  /**
   * Child Form Elements
   * 
   * @var FormElement[]
   */
  public $children;
  /**
   * Child Form Controls.
   * 
   * @var FormControl[]
   */
  public $controls;

  /**
   * Creates a Empty Form Element.
   */
  public function __construct() {
    $this->children = array();
    $this->controls = array();
  }

  /**
   * Takes a function applies it to each child.
   */
  public function eachChild($func) {
    
  }

  /**
   * Takes a function applies it to each control.
   */
  public function eachControl() {
    
  }

  /**
   * Returns an iterator that iterates over this elements decendants. Children and children of children.
   * 
   * @return iterator
   */
  public function eachDecendant() {
    
  }

  /**
   * 
   */
  public function __toString() {
    // For each control and each child, treat them as a string pack into an array and do a print_r.
  }

  /**
   * Gets a child or control if found.
   * 
   * @param mixed $name 
   * @return mixed
   */
  public function __get($name) {
    if ($this->offsetExists($name)) {
      return $this->offsetGet($name);
    }
    else if ($this->offsetExists("#$name")) { // Check controls with the leading '#'
      return $this->offsetGet("#$name");
    }
    return NULL;
  }

  /**
   * Sets a child or control.
   *  
   * @param mixed $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $name = is_decendant($value, 'FormControl') ? "#$name" : $name;
    $this->offsetSet($name, $value);
  }

  /**
   * Checks if a given control or child exists.
   * 
   * @param mixed $name 
   * 
   * @return boolean
   */
  public function __isset($name) {
    return ($this->$name != NULL);
  }

  /**
   * Removes a child or control if found.
   * 
   * @param mixed $name 
   */
  public function __unset($name) {
    if ($this->offsetExists($name)) {
      $this->offsetUnset($name);
    }
    else if ($this->offsetExists("#$name")) { // Check controls with the leading '#'
      $this->offsetUnset("#$name");
    }
  }

  /**
   * Checks to see if any child or control exists for the given offset.
   * 
   * @param mixed $offset 
   * @return boolean
   */
  public function offsetExists($offset) {
    $child_exists = array_key_exists($offset, $this->children);
    $control_exists = array_key_exists($offset, $this->controls);
    if ($child_exists || $control_exists) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Looks for a child or control for the the given offset.
   * 
   * @param mixed $offset 
   * @return mixed
   *   If found, this function will return either a child FormElement or a FormControl.
   */
  public function offsetGet($offset) {
    $child_exists = array_key_exists($offset, $this->children);
    $control_exists = array_key_exists($offset, $this->controls);
    if ($child_exists) {
      return $this->children[$offset];
    }
    else if ($control_exists) {
      return $this->control[$offset];
    }
    return NULL;
  }

  /**
   * Sets a child or control defined by $value.
   * 
   * @param mixed $offset
   * @param mixed $value 
   *   Either a FormElement or a FormControl, all other values will be ignored.
   */
  public function offsetSet($offset, $value) {
    if (is_decendant($value, 'FormElement')) {
      $this->setChild($offset, $value);
    }
    else if (is_decendant($value, 'FormControl')) {
      $this->setControl($offset, $value);
    }
  }

  /**
   * Removes the child or control identified by $offset if found.
   * 
   * @param mixed $offset 
   */
  public function offsetUnset($offset) {
    $child_exists = array_key_exists($offset, $this->children);
    $control_exists = array_key_exists($offset, $this->controls);
    if ($child_exists) {
      unset($this->children[$offset]);
    }
    else if ($control_exists) {
      unset($this->control[$offset]);
    }
  }

  /**
   * Sets a child to this form element.
   * 
   * @param mixed $offset
   * @param FormElement $child 
   */
  private function setChild($offset, FormElement $child) {
    $value->setParent($parent);
    if (isset($offset)) {
      $this->children[$offset] = $value;
    }
    else {
      $this->children[] = $value;
    }
  }

  /**
   * Sets a control to this form element.
   * 
   * @param mixed $offset
   * @param FormControl $control 
   */
  private function setControl($offset, FormControl $control) {
    if (isset($offset)) {
      $this->control[$offset] = $value;
    } // Controls are not indexed values EVAR!.
  }

}