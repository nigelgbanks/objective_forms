<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'ReflectionHelpers');
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'objective_forms', 'Utils');

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
   * @var array
   */
  public $children;
  /**
   * Child Form Controls.
   * 
   * @var array
   */
  public $controls;

  /**
   * Creates a Empty Form Element.
   * 
   * @param array $controls
   * @param array $children
   */
  public function __construct(array $form = NULL) {
    $this->protected = new ReadOnlyProtectedMembers(array('parent' => NULL, 'hash' => NULL));
    $this->controls = array();
    $this->children = array();
    $this->initialize($form);
    $this->hash = isset($this->controls['#hash']) ? $this->controls['#hash'] : spl_object_hash($this);
    module_invoke_all('form_element_created', $this); // Some modules will want to register all created FormElements.
  }

  /**
   * Initialize this object from a drupal form definition.
   * 
   * @param array $form 
   */
  protected function initialize(array &$form = NULL) {
    if (isset($form)) {
      $this->initializeControls($form);
      $this->initializeChildren($form);
    }
  }

  /**
   * Create controls from drupal form definition.
   * 
   * @param array $form
   */
  protected function initializeControls(array &$form) {
    module_load_include('inc', 'objective_forms', 'FormProperty');
    $properties = element_properties($form);
    foreach ($properties as $key) {
      $this->controls[$key] = FormProperty::Expand($key, $form[$key]);
    }
  }

  /**
   * Create children from drupal form definition.
   * 
   * @param array $form 
   */
  protected function initializeChildren(array &$form) {
    $children = element_children($form);
    foreach ($children as $key) {
      $child = new FormElement($form[$key]);
      $this->adopt($key, $child);
    }
  }

  /**
   * Clone this FormElement. 
   * 
   * Performs a deep clone on controls and children. The cloned element wont have a parent.
   */
  public function __clone() {
    $original_hash = $this->hash;
    $this->protected = clone $this->protected;
    $this->parent = NULL; // The parent was unaware of this cloned bastard child of science...
    $this->hash = spl_object_hash($this); // Maintain uniqueness.
    foreach ($this->controls as $key => $control) { // Clone Controls.
      if (is_array($control)) {
        $this->controls[$key] = array_copy_recursive($control);
      }
      else if (is_object($control)) {
        $this->controls[$key] = clone $control;
      }
    }
    foreach ($this->children as $key => $child) { // Clone Children.
      $this->children[$key] = clone $child;
    }
    module_invoke_all('form_element_cloned', $original_hash, $this); // Some modules will want to register all cloned FormElements.
  }

  /**
   *
   * @param hash $hash 
   * @return FormElement
   */
  public function findElement($hash) {
    if ($this->hash == $hash) {
      return $this;
    }
    foreach ($this->children as $child) {
      $element = $child->findElement($hash);
      if ($element) {
        return $element;
      }
    }
    return NULL;
  }

  /**
   * Takes a function applies it to each child.
   * 
   * @param type $user_func 
   */
  public function eachChild($user_func) {
    $original_args = func_get_args();
    $user_func_args = $original_args;
    array_shift($user_func_args); // Remove user function from the argument list
    foreach ($this->children as $child) {
      if ($this->callUserFunction($user_func, $child, $user_func_args) === FALSE) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Call a user function.
   * 
   * @param string $user_func
   * @param mixed $first_argument
   * @param array $user_func_args
   * 
   * @return boolean
   */
  private function callUserFunction($user_func, $first_argument, array &$user_func_args) {
    array_unshift($user_func_args, $first_argument);
    $ret = call_user_func_array($user_func, $user_func_args);
    array_shift($user_func_args);
    return $ret;
  }

  /**
   * Takes a function applies it to each control.
   */
  public function eachControl($user_func) {
    $user_func_args = func_get_args();
    array_shift($user_func_args); // Remove user function from the argument list
    foreach ($this->controls as $key => &$control) {
      if ($this->callUserFunction($user_func, array($key, &$control), $user_func_args) === FALSE) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * 
   * 
   * @return iterator
   */
  public function eachDecendant($user_func) {
    $original_args = func_get_args();
    $user_func_args = $original_args;
    array_shift($user_func_args); // Remove user function from argument list.
    foreach ($this->children as $child) {
      if ($this->callUserFunction($user_func, $child, $user_func_args) === FALSE) {
        return FALSE; // End processing
      }
      if (call_user_func_array(array($child, 'eachDecendant'), $original_args) === FALSE) {
        return FALSE; // End processing
      }
    }
    return TRUE; // Continue processing
  }

  /**
   * 
   */
  public function each($user_func) {
    $original_args = func_get_args();
    $user_func_args = $original_args;
    array_shift($user_func_args); // Remove user function from argument list.
    if ($this->callUserFunction($user_func, $this, $user_func_args) === TRUE) {
      return call_user_func_array(array($this, 'eachDecendant'), $original_args);
    }
    return FALSE;
  }

  /**
   * Gets a child or control if found.
   * 
   * @param mixed $name 
   * @return mixed
   */
  public function __get($name) {
    if ($this->protected->has($name)) {
      return $this->protected->$name;
    }
    else if ($this->offsetExists($name)) { // Child
      return $this->offsetGet($name);
    }
    else if ($this->offsetExists("#$name")) { // Control
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
    if ($this->protected->has($name)) {
      $this->protected->$name = $value;
    }
    else {
      $name = is_or_descends_from($value, 'FormElement') ? $name : "#$name";
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
    if ($this->protected->has($name)) {
      return isset($this->protected->$name);
    }
    return $this->offsetExists($name) || $this->offsetExists("#$name");
  }

  /**
   * Removes a child or control if found.
   * 
   * @param mixed $name 
   */
  public function __unset($name) {
    if ($this->protected->has($name)) {
      unset($this->protected->$name);
    }
    else if ($this->offsetExists($name)) { // Child
      $this->offsetUnset($name);
    }
    else if ($this->offsetExists("#$name")) { // Control
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
    $child_exists = isset($this->children[$offset]);
    $control_exists = isset($this->controls[$offset]);
    return $child_exists || $control_exists;
  }

  /**
   * Looks for a child or control for the the given offset.
   * 
   * @param mixed $offset 
   * @return mixed
   *   If found, this function will return either a child FormElement or a FormControl.
   */
  public function offsetGet($offset) {
    // Children
    $child_exists = array_key_exists($offset, $this->children);
    if ($child_exists) {
      return $this->children[$offset];
    }
    // Controls
    $control_exists = array_key_exists($offset, $this->controls);
    if ($control_exists) {
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
    else if (element_property($offset)) {
      $this->controls[$offset] = $value;
    }
  }

  /**
   * Removes the child or control identified by $offset if found.
   * 
   * @param mixed $offset 
   */
  public function offsetUnset($offset) {
    if (isset($this->children[$offset])) {
      unset($this->children[$offset]);
    }
    else if (isset($this->controls[$offset])) {
      unset($this->controls[$offset]);
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
    $this->orphan(); // There can only be one.
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

  /**
   *
   * @return array
   */
  public function toArray() {
    $output = array('#hash' => $this->hash);
    $this->addControlsToArray($output);
    $this->addChildrenToArray($output);
    return $output;
  }

  /**
   *
   * @param array $output 
   */
  private function addControlsToArray(array &$output) {
    foreach ($this->controls as $name => $control) {
      if (has_interface($control, 'FormPropertyInterface')) {
        $output[$name] = $control->getValue();
      }
      else {
        $output[$name] = $control;
      }
    }
  }

  /**
   *
   * @param array $output 
   */
  private function addChildrenToArray(array &$output) {
    foreach ($this->children as $name => $child) {
      $output[$name] = $child->toArray();
    }
  }

}