<?php
// $Id$

/**
 * Form definition function for the settings form.
 * @link http://api.drupal.org/api/function/hook_form/6
 */
function richmedia_upload_settings_form() {
  $form['richmedia_upload_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable richmedia_upload'),
    '#default_value' => variable_get('richmedia_upload_enabled', FALSE),
  );

  return system_settings_form($form);
}