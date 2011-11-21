<?php
// $Id$

/**
 * Form definition function for the settings form.
 * @link http://api.drupal.org/api/function/hook_form/6
 */
function richmedia_import_settings_form() {
  $form = array();
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => 'Import settings',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['import']['richmedia_import_uploadpath'] = array(
    '#type' => 'textfield',
    '#title' => t('Upload path'),
    '#description' => t('Place to save the uploaded presentations in. e.g. /sites/default/files'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => variable_get('richmedia_import_uploadpath', file_directory_path().'/uploaded'),
  );

  $form['import']['richmedia_import_unpackpath'] = array(
    '#type' => 'textfield',
    '#title' => t('Unpack path'),
    '#description' => t('While processing the files, unpack them to this path.'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => variable_get('richmedia_import_unpackpath', file_directory_path().'/unpacked'),
  );

  $profiles = richmedia_import_get_profiles();

  $form['import']['richmedia_import_default_transcode_profile'] = array(
    '#type' => 'select',
    '#title' => t('Default Transcode Profile'),
    '#options' => $profiles,
    '#default_value' => variable_get('richmedia_import_default_transcode_profile', 0),
  );

  $form['#submit'][] = 'richmedia_import_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Implementation of hook_submit().
 * @todo check if the FILE_CREATE_DIRECTORY boolean doesn't need to be changed. (create now, maybe should be change permissons)
 */
function richmedia_import_settings_form_submit(&$form, &$form_state) {
  file_check_directory($form_state['values']['richmedia_import_uploadpath'], FILE_CREATE_DIRECTORY, 'richmedia_import_uploadpath');
  file_check_directory($form_state['values']['richmedia_import_unpackpath'], FILE_CREATE_DIRECTORY, 'richmedia_import_unpackpath');
}

/**
 * Helper function to retrieve the profiles and put em into a decent array.
 */
function richmedia_import_get_profiles($reset = FALSE) {
  // Include functions
  module_load_include('inc', 'richmedia');

  // The used username
  $user_id = variable_get('vpx_connector_username', NULL);

  if ($reset) {
    variable_del('richmedia_import_profiles');
  }

  if (!$profile = variable_get('richmedia_import_profiles', FALSE)) {
    if ($user_id) {
      $xml = richmedia_api_get_transcode_profiles($user_id);
      $raw_items = $xml->items->item;

      foreach($raw_items as $key => $values) {
        $items[(int) $values->profile_id] = (string) $values->profile;
      }
    }

    variable_set('richmedia_import_profiles', $items);
    $profile = $items;
  }

  return $profile;
}