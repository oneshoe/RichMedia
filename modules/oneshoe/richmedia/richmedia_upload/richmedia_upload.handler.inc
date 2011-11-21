<?php
// $Id$

// Set some constants.
define('NUMBER_MEDIAFILES', 2);
define('WLE_UPLOAD_STILL_PER_MEDIAFILE', 3);
define('WLE_UPLOAD_STILL_EVERY_SECOND', 5);
define('WLE_UPLOAD_START_FRAME', 0);
define('WLE_UPLOAD_END_FRAME', 9999999);

/**
 * @file
 * File containing upload handling functions and form functionality.
 */
function richmedia_handler_create_mediafile($node, $user_id, $total) {
  // Default values
  $media_files = array();

  // Include functions
  module_load_include('inc', 'richmedia');

  // Check if there are mediafile's in the database and put them in an Array;
  // @Todo Check if this file is called multiple time in a script, is so use static.
  $result = db_query("SELECT * FROM {richmedia_media} WHERE type = 'VIDEO' AND nid = %d", $node->nid);
  while ($record = db_fetch_object($result)) {
    $media_files[$record->placement] = $record;
  }

  // Count if we even have media_files
  $count = count($media_files);
  if ($count) {
    // Get the max the placement & total number of media_files we need to create.
    $max_placement = max(array_keys($media_files)) + 1;
    $create = $total - $count;
  }
  else {
    // No media_files found, we are gonna set placement on 0 and create em all.
    $max_placement = 0;
    $create = $total;
  }

  // if we have more then 0 items to create, create em.
  // Specific on > 0, to ensure mistakes on -1 dont get through
  if ($create > 0) {
    for ($x = 0; $x < $create; $x++) {
      // Create a mediafile_id.
      $media_file = richmedia_api_create_mediafile($node->richmedia->asset_id, $user_id);
      // Create the object for the array and database.
      $object = (object) array(
        'nid' => $node->nid,
        'mediafile_id' => $media_file,
        'type' => 'VIDEO',
        'placement' => $max_placement,
        'timecode' => 0,
        'status' => 0,
      );
      // Store the mediafile in the database
      _richmedia_mediafile_store($node->nid, $media_file, $object);
      $media_files[$max_placement] = $object;
      $max_placement++;
    }
  }

  return $media_files;
}



/**
 * Handler function for the upload forms.
 */
function richmedia_handler_upload_page($node = NULL, $handler = NULL) {
  /**
   * Check if we get extra arguments.
   */
  if (isset($_GET['upload_ticket'])) {
    $mediafile_id = db_result(db_query_range("SELECT mediafile_id FROM {richmedia_upload} WHERE ticket = '%s'", $_GET['upload_ticket'], 0, 1));
    if ($mediafile_id) {
      // Delete the record so we can only upload once.
      db_query("DELETE FROM {richmedia_upload} WHERE ticket = '%s'", $_GET['upload_ticket']);
      db_query("UPDATE {richmedia_media} SET status = 1 WHERE mediafile_id = '%s' AND nid = %d", $mediafile_id, $node->nid);
    }
  }

  // The url used in the upload
  $url = variable_get('vpx_connector_uri', NULL);

  $user_id = variable_get('vpx_connector_username', NULL);

  // Default values
  $todo = array();

  // Get all the media_files we need on this node, create them if needed.
  $media_files = richmedia_handler_create_mediafile($node, $user_id, NUMBER_MEDIAFILES);

  // Create an upload ticket for the files that need it.
  foreach($media_files as $placement => $mediafile_record) {
    if (!$mediafile_record->status) {
      // Create the ticket needed
      $upload_ticket = _richmedia_upload_check_uploadticket($node->nid, $mediafile_record->mediafile_id);
      $upload_ticket->action = url($url .'/mediafile/upload', array('query' => array('upload_ticket' => $upload_ticket->ticket)));
      $todo[] = $upload_ticket;
    }
  }
  $state = array('primary' => 'richmedia-upload-primary inactive', 'secondary' => 'richmedia-upload-secondary inactive', 'tertiary' => 'richmedia-upload-tertiary inactive');
  switch(count($todo)) {
    case 1:
      $state['secondary'] = 'richmedia-upload-secondary active';
      $primary = NULL;
      $secondary = array_shift($todo);
    break;
    case 2:
    default:
      $state['primary'] = 'richmedia-upload-primary active';
      $primary = array_shift($todo);
      $secondary = array_shift($todo);
    break;
  }

  $primary_form = drupal_get_form('richmedia_handler_upload_primary', $node, $primary);
  $secondary_form = drupal_get_form('richmedia_handler_upload_secondary', $node, $secondary);
  $output = '';
  $output = theme('richmedia_handler_upload_page', $node, $primary_form, $secondary_form, $state);
  return $output;
}

/**
 * Theme function for displaying the upload page with 3 forms.
 */
function theme_richmedia_handler_upload_page($node, $form_primary, $form_secondary, $state) {

  drupal_add_css(drupal_get_path('module', 'richmedia_upload') .'/css/richmedia_upload.css');
  drupal_add_js(drupal_get_path('module', 'richmedia_upload') .'/js/richmedia_upload.js');

  $output = '';
  $output .= '<div class="richmedia-upload-area"><div class="richmedia-upload-inner">';

  $output .= '<div class="richmedia-upload-form '. $state['primary'] .'"><h3>'. t('Step 1') .'</h3>';
  $output .= $form_primary;
  $output .= '</div>';

  $output .= '<div class="richmedia-upload-form '. $state['secondary'] .'"><h3>'. t('Step 2') .'</h3>';
  $output .= $form_secondary;
  $output .= '</div>';

  $output .= '</div></div>';
  return $output;
}

function richmedia_handler_upload_primary($form_state, $node, $upload_ticket) {
  $options = array(
    'class' => 'upload-primary',
    'ticket' => $upload_ticket->ticket,
    'action' => $upload_ticket->action,
    'mediafile_id' => $upload_ticket->mediafile_id,
    'redirect' => url($_REQUEST['q'], array('absolute' => TRUE, 'query' => array('upload_ticket' => $upload_ticket->ticket))),
  );
  return richmedia_upload_form($options);
}


function richmedia_handler_upload_secondary($form_state, $node, $upload_ticket) {
  $options = array(
    'class' => 'upload-secondary',
    'ticket' => $upload_ticket->ticket,
    'action' => $upload_ticket->action,
    'mediafile_id' => $upload_ticket->mediafile_id,
    'redirect' => url('node/'. $node->nid .'/check/'.$upload_ticket->ticket, array('absolute' => TRUE)),
  );
  $form = array();
  $form += richmedia_handler_still_creation_form($form_state, $node);
  $form += richmedia_upload_form($options);

  return $form;
}

function richmedia_handler_still_creation_form($form_state, $node) {
  $form = array();
  $form['create_still'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create stills'),
    '#description' => t('Please select if you want to create stills'),
    '#default_value' => 0,
    '#return_value' => 'true',
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => FALSE,
    '#attributes' => array('class' => 'still_creation_options inactive'),
  );

  $form['options']['still_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of creating stills'),
    '#default_value' => 'NONE',
    '#options' => array(
      'NONE' => t('NONE - original behaviour'),
      'NORMAL' => t('NORMAL - x stills per mediafile'),
      'SECOND' => t('SECOND - one still every x seconds'),
      'SCENE' => t('SCENE - one still for each scene'),
    ),
  );
  $form['options']['still_per_mediafile'] = array(
    '#type' => 'textfield',
    '#title' => t('Still per mediafile'),
    '#default_value' => WLE_UPLOAD_STILL_PER_MEDIAFILE,
    '#description' => 'For NORMAL stills',
    '#attributes' => array('class' => 'normal'),
  );
  $form['options']['still_every_second'] = array(
    '#type' => 'textfield',
    '#title' => t('Still on every seconds'),
    '#default_value' => WLE_UPLOAD_STILL_EVERY_SECOND,
    '#description' => 'For SECOND stills',
    '#attributes' => array('class' => 'second'),
  );
  $form['options']['start_frame'] = array(
    '#type' => 'textfield',
    '#title' => t('Start frame'),
    '#default_value' => WLE_UPLOAD_START_FRAME,
    '#description' => 'For NORMAL and SECOND stills',
    '#attributes' => array('class' => 'second normal'),
  );
  $form['options']['end_frame'] = array(
    '#type' => 'textfield',
    '#title' => t('End frame'),
    '#default_value' => WLE_UPLOAD_END_FRAME,
    '#description' => 'For NORMAL and SECOND stills',
    '#attributes' => array('class' => 'second normal'),
  );

  return $form;
}

/**
 * Check upload functions.
 */
function richmedia_upload_check_upload($node, $upload_ticket = NULL) {
  if (isset($upload_ticket)) {
    // Get the mediafile_id which is connected to this upload ticket and remove it.
    $mediafile_id = db_result(db_query_range("SELECT mediafile_id FROM {richmedia_upload} WHERE ticket = '%s'", $upload_ticket, 0, 1));
    if ($mediafile_id) {
      // Delete the record so we can only upload once.
      db_query("DELETE FROM {richmedia_upload} WHERE ticket = '%s'", $upload_ticket);
      db_query("UPDATE {richmedia_media} SET status = 1 WHERE mediafile_id = '%s' AND nid = %d", $mediafile_id, $node->nid);
    }

    // Set the database to the correct value so this tab is gone.
    db_query("UPDATE {richmedia_asset} SET status = 2 WHERE nid = %d", $node->nid);

    // Get the preload image array and set the new NID to it.
    $preload_images = variable_get('richmedia_upload_preload_images', array());
    $preload_images[$node->nid] = 1;
    variable_set('richmedia_upload_preload_images', $preload_images);
  }

  drupal_goto('node/'. $node->nid);
}