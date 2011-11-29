<?php

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'Array');

/**
 * Recursively searches the Drupal form for the element with the given hash.
 * 
 * @param array $form 
 *   The Drupal Form.
 * 
 * @param hash $hash
 *   The unique #hash property that identifies the FormElement.
 */
function &find_element(array &$form, $hash) {
  if (has_hash($form, $hash)) {
    return $form;
  }
  return find_child_element($form, $hash);
}

/**
 * Recursively searches the Drupal Form elements children for the element that matches the given hash.
 * 
 * @param array $form
 *   A Drupal Form Element.
 * @param hash $hash
 *   The unique #hash property that identifies the FormElement.
 * 
 * @return array
 *   The drupal form element whos #hash matches that of the given $hash value.
 */
function &find_child_element(array &$form, $hash) {
  $found = NULL;
  $children = element_children($form);
  foreach ($children as $key) {
    $child = &$form[$key];
    $found = has_hash($child, $hash) ? $child : find_child_element($child, $hash);
    if (isset($found)) {
      return $found;
    }
  }
  return $found;
}

/**
 * Checks to see if the given drupal form element has a #hash property and checks 
 * to see if it matches the given hash.
 * 
 * @param array $element
 *   A Drupal Form Element.
 * @param hash $hash
 *   The unique #hash property that identifies the FormElement.
 * 
 * @return boolean
 *   TRUE if the given element's hash matches, FALSE otherwise.
 */
function has_hash(array &$element, $hash) {
  if (isset($element['#hash']) && $element['#hash'] == $hash) {
    return TRUE;
  }
  return FALSE;
}