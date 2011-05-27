<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'ReflectionHelpers');
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'objective_forms', 'Utils');
module_load_include('inc', 'objective_forms', 'FormControl');

/**
 * Form Element Class.
 */
class FormElement implements ArrayAccess {

  /**
   * Holds references to protected variables. Allows for external access.
   * 
   * @var array
   */
  protected $protected;
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
  public function __construct(array $controls = NULL, array $children = NULL) {
    $this->controls = isset($controls) ? $controls : array();
    $this->children = isset($children) ? $children : array();
    $this->protected = new ReadOnlyProtectedMembers(array('parent' => NULL, 'hash' => spl_object_hash($this)));
    module_invoke_all('form_element_created', $this); // Some modules will want to register all created FormElements.
  }

  /**
   * Called when this object is destroyed.
   */
  public function __destruct() {
    module_invoke_all('form_element_destroyed', $this->hash); // Some modules will want to unregister any destroyed FormElements.
  }

  /**
   * Clone this FormElement. 
   * 
   * Performs a deep clone on controls and children. The cloned element wont have a parent.
   */
  public function __clone() {
    $original_hash = $this->hash;
    $this->hash = spl_object_hash($this); // Maintain uniqueness.
    $this->parent = NULL; // The parent was unaware of this cloned bastard child of science...
    foreach ($this->control as $key => $control) { // Clone Controls.
      $this->control[$key] = clone $control;
    }
    foreach ($this->children as $key => $child) { // Clone Children.
      $this->children[$key] = clone $child;
    }
    module_invoke_all('form_element_cloned', $original_hash, $this); // Some modules will want to register all cloned FormElements.
  }

  /**
   * Takes a function applies it to each child.
   */
  public function eachChild($function) {
    $args = func_get_args();
    array_shift($args); // Remove function argument
    foreach ($this->children as $child) {
      array_unshift($child, $args);
      if (call_user_func_array($function, $args) == FALSE) {
        return;
      }
      array_shift($args);
    }
  }

  /**
   * Takes a function applies it to each control.
   */
  public function eachControl($function) {
    $args = func_get_args();
    array_shift($args); // Remove function argument
    foreach ($this->control as $control) {
      array_unshift($control, $args);
      if (call_user_func_array($function, $args) == FALSE) {
        return;
      }
      array_shift($args);
    }
  }

  /**
   * Returns an iterator that iterates over this elements decendants. Children and children of children.
   * 
   * @return iterator
   */
  public function eachDecendant($function) {
    $args = func_get_args();
    array_shift($args); // Remove function argument
    foreach ($this->children as $child) {
      if (call_user_func_array($function, $args) == FALSE) {
        return;
      }
    }
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
    if ($this->protected->exists($name)) {
      return $this->protected->get($name);
    }
    else if ($this->offsetExists($name)) {
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
    if ($this->protected->exists($name)) {
      return $this->protected->set($name, $value);
    }
    else {
      $name = is_or_descends_from($value, 'FormControl') ? "#$name" : $name;
      $this->offsetSet($name, $value);
    }
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
      return $this->controls[$offset];
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
    if (is_or_descends_from($value, 'FormElement')) {
      $this->adopt($offset, $value);
    }
    else if (is_or_descends_from($value, 'FormControl')) {
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
   * Sets a control to this form element.
   * 
   * @param mixed $offset
   * @param FormControl $control 
   */
  private function setControl($offset, FormControl $control) {
    if (isset($offset)) {
      $this->controls[$offset] = $control;
    } // Controls are not indexed values EVAR!.
  }

  /**
   * Adopt the given child. Forcefully removing from its original home.
   * 
   * @param mixed $offse
   * @param FormElement $child
   */
  private function adopt($offset = NULL, FormElement $child) {
    $child->setParent($this);
    if (isset($offset)) {
      $this->children[$offset] = $child;
    }
    else {
      $this->children[] = $child;
    }
    $child->parent = $this;
  }

  /**
   * Sets this elements parent, ditching its previous parents.
   * 
   * @param FormElement
   */
  private function setParent(FormElement $parent) {
    $this->orphan();
    $this->parent = $parent;
  }

  /**
   * Removes this element from its parent element.
   * 
   * @return boolean
   *   TRUE if the element was orphaned FALSE otherwise.
   */
  public function orphan() {
    if (isset($this->parent)) {
      $was_orphaned = $this->parent->orphanChild($this);
      unset($this->parent);
      return $was_orphaned;
    }
    return FALSE;
  }

  /**
   * Removes the child element defined by $remove from this FormElement. 
   * 
   * @param FormElement $remove
   *   The child to remove
   * 
   * @return boolean
   *   TRUE if the child was removed, FALSE otherwise.
   */
  private function orphanChild(FormElement $remove) {
    foreach ($this->children as $index => $child) {
      if ($remove === $child) {
        unset($this->children[$index]);
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Get's the index of this element in its parent's children array if it has a parent.
   */
  public function getIndex() {
    if (isset($this->parent)) {
      foreach ($this->parent->children as $index => $child) {
        if ($child === $this) {
          return $index;
        }
      }
    }
    return NULL;
  }

  public function toArray() {
    $output = array();
    foreach ($this->controls as $control) {
      $output[$control->name];
    }
  }

}