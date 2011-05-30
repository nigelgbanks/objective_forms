<?php

// $Id$

/**
 * @file
 *
 */

/**
 * An interface for FormProperties that want to be treated as objects.
 */
interface FormPropertyInterface {
  public function toDrupalForm();
}