<?php

// $Id$

/**
 * @file
 *
 */
class FormStorage {
  /**
   * The root element where we store all the required info.
   */
  const STORAGE_ROOT = 'objective_form_storage';

  /**
   * This instance.
   * 
   * @var Registry
   */
  private static $instance = NULL;
  /**
   * Spot to keep our stuff.
   * 
   * @var array 
   */
  protected $storage;

  /**
   * Initializes the FormStorage singleton, only needs to be called once.
   * 
   * @param array $form_state
   * 
   * @return FormStorage
   */
  public static function initialize(array &$form_state) {
    if (!self::isInitialized()) {
      self::$instance = new FormStorage($form_state);
    }
    return self::$instance;
  }

  /**
   * Checks if the FormStorage singleton has been initialized.
   * 
   * @return boolean
   */
  public static function isInitialized() {
    return isset(self::$instance);
  }

  /**
   * Gets the FormStorage singleton instance.
   * 
   * @return FormStorage
   */
  public static function getInstance() {
    if (!self::isInitialized()) {
      throw new Exception('Attempted to get FormStorage instance without first initializing it.');
    }
    return self::$instance;
  }

  /**
   * Creates the FormStorage Singleton
   * 
   * @param array $form_state 
   */
  private function __construct(array &$form_state) {
    $this->initializeFormState($form_state);
    $this->storage = &$form_state['storage'];
  }

  /**
   * Creates a storage slot in the Drupal form state.
   * 
   * @param array $form_state 
   */
  private function initializeFormState(array &$form_state) {
    if (empty($form_state['storage'])) {
      $form_state['storage'] = array();
    }
    if (empty($form_state['storage'][self::STORAGE_ROOT])) {
      $form_state['storage'][self::STORAGE_ROOT] = array();
    }
  }

  /**
   * Checks storage for the variable identified by name.
   * 
   * @param string $name
   * @return boolean
   */
  public function __isset($name) {
    return isset($this->storage[$name]);
  }

  /**
   * Removes a variable from storage.
   * 
   * @param string $name 
   */
  public function __unset($name) {
    unset($this->storage[$name]);
  }

  /**
   * Get a value from storage.
   * 
   * @param mixed $name
   * @return mixed 
   */
  public function __get($name) {
    if (isset($this->storage[$name])) {
      return $this->storage[$name];
    }
    return NULL;
  }

  /**
   * Store a value.
   * 
   * @param mixed $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $this->storage[$name] = $value;
  }

}