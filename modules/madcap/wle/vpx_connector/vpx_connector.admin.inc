<?php
// $Id$

/**
 * @file
 * Separate file for admin settings form.
 */

/**
 * VPX connector settings
 */
function vpx_connector_settings() {
  $max_result_set = 200;

  $form = array();

  $form['connection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection settings'),
  );
  if (function_exists('madrest_get_uri_definition')) {
    $form['connection']['vpx_use_rest_interface'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the VP-X REST interface'),
      '#description' => t('Unchecking this item requires a working copy of VP-X at this instance of Drupal.'),
      '#default_value' => variable_get('vpx_use_rest_interface', TRUE),
    );
    $form['connection']['vpx_connector_masquerade_owner'] = array(
      '#type' => 'checkbox',
      '#title' => t("Always use the owner's credentials to perform an action on an item."),
      '#description' => t('Use this option with care!'),
      '#default_value' => variable_get('vpx_connector_masquerade_owner', FALSE),
    );
  }
  $form['connection']['vpx_connector_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('vpx_connector_username', ''),
  );
  $form['connection']['vpx_connector_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('vpx_connector_password', ''),
  );
  $form['connection']['vpx_connector_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => variable_get('vpx_connector_uri', ''),
  );

  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other settings'),
  );
  $form['other']['vpx_connector_item_limit'] = array(
    '#type' => 'textfield',
    '#description' => t('Maximum: @max', array('@max' => $max_result_set)),
    '#maxlength' => 3,
    '#size' => 3,
    '#title' => t('Maximum result set'),
    '#default_value' => variable_get('vpx_connector_item_limit', $max_result_set),
  );
  $form['other']['vpx_connector_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show VP-X request information'),
    '#default_value' => variable_get('vpx_connector_debug', FALSE),
  );
  $form['other']['vpx_connector_menu_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu prefix'),
    '#description' => t('Use a training slash and please remember to clear the !cache after changing the menu prefix!', array('!cache' => l(t('cache'), 'admin/settings/performance'))),
    '#default_value' => variable_get('vpx_connector_menu_prefix', ''),
  );

  $form['#validate'] = array('vpx_connector_settings_validate');

  return system_settings_form($form);
}

function vpx_connector_settings_validate(&$form, &$form_state) {
  variable_set('vpx_connector_cookie', NULL);

  $vpx = new vpx_connector();
  $result = $vpx->request('GET', '/asset?limit=1');

  if ($result !== FALSE) {
    drupal_set_message(t('Login test successful. VP-X version @version.', array('@version' => (string)$result->header->version)));
  }
  else {
    drupal_set_message(t('Login test failed!'), 'error');
  }
}

/**
 * VPX connector handler
 */
function _vpx_connector_settings_submit(&$form_state , $form) {
  foreach (array('vpx_connector_username', 'vpx_connector_password', 'vpx_connector_uri', 'vpx_connector_item_limit') as $subject) {
    variable_set($subject, $form['values'][$subject]);
  }
  foreach (array('vpx_connector_debug', 'vpx_connector_masquerade_owner') as $subject) {
    variable_set($subject, isset($form['values'][$subject]));
  }
  variable_set('vpx_connector_cookie', NULL);
}