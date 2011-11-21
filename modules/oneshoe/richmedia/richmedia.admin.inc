<?php
// $Id$

/**
 * Form definition function for the settings form.
 * @link http://api.drupal.org/api/function/hook_form/6
 */
function richmedia_settings_form() {

  $form['workflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['workflow']['richmedia_asset_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-create asset'),
    '#description' => t('Auto-create a VP-Core asset when no asset id is linked to the content item. Auto-creating an asset with asset ID will be executed on adding/updating a content item when the content item does not already have an asset ID assigned to it.'),
    '#default_value' => variable_get('richmedia_asset_create', TRUE),
  );

  $form['workflow']['richmedia_asset_delete'] = array(
    '#type' => 'radios',
    '#title' => t('Delete workflow'),
    '#description' => t('When a Drupal content item is deleted, what action should be taken with the externally hosted VP-Core asset?'),
    '#default_value' => variable_get('richmedia_asset_delete', 0),
    '#options' => array(
      '' => t('Do not delete the externally stored asset.'),
      RICHMEDIA_DELETE_ASSET => t('Delete the asset only when no mediafiles are connected.'),
      RICHMEDIA_DELETE_ASSET_RECURSIVE => t('Delete the complete asset including all mediafiles and stills.'),
    ),
  );

  $form['workflow']['richmedia_asset_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group name'),
    '#description' => t('The group name to use for organizing Rich Media assets.'),
    '#default_value' => variable_get('richmedia_asset_group', 'richmedia'),
    '#maxlength' => 255,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array(
    RICHMEDIA_DEBUG_WATCHDOG => t('Write to Watchdog'),
    RICHMEDIA_DEBUG_MESSAGE => t('Display debug status messages'),
    RICHMEDIA_DEBUG_ADVANCED => t('Advanced debug information in messages'),
  );

  $form['advanced']['richmedia_debug'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Debug mode'),
    '#options' => $options,
    '#default_value' => richmedia_expand_bit(variable_get('richmedia_debug', 0), array_keys($options)),
    '#description' => t('Here you can set the site debug mode for Rich Media functionality.'),
  );
  return system_settings_form($form);
}

/**
 * Validation callback for the richmedia_settings_form().
 */
function richmedia_settings_form_validate(&$form, &$form_state) {
  $form_state['values']['richmedia_debug'] = array_sum($form_state['values']['richmedia_debug']);
}

/**
 * Function for expanding a number value to separate bits using an array of parts.
 *
 * For example, providing
 * <code>richmedia_expand_bit(7, array(1, 2, 4, 8, 16));</code>
 * will return an array like:
 * <code>array(1, 2, 4);</code>
 *
 * @param int $value The numeric value containing several bits.
 * @param array $options Array containing numeric parts as values.
 *
 * @return Returns an array on success, FALSE on error (not providing an array for
 *         the second parameter for example.
 */
function richmedia_expand_bit($value, $options) {
  $return = array();
  if (!is_array($options)) return FALSE;
  $value = (int)$value;

  foreach ($options as $bit) {
    if ($value & $bit) $return[] = $bit;
  }
  return $return;
}