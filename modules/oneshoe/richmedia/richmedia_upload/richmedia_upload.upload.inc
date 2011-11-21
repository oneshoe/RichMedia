<?php
// $Id$

/**
 * @file
 * Upload page and functionality for the richmedia_upload module.
 */

/**
 * Page callback for the upload page.
 */
function richmedia_upload_page($node, $identifier = NULL) {
  $handlers = _richmedia_get_upload_handlers($node);

  if (!$handlers) {
    drupal_set_message(t('There are no upload handlers available. Uploading cannot continue.'), 'error');
  }

  if (!$identifier && count($handlers) == 1) {
    $identifier = (key($handlers));
  }
  elseif (!($identifier && isset($handlers[$identifier]))) {
    return theme('richmedia_upload_choice', $handlers, 'node/'. $node->nid .'/upload');
  }

  $handler = $handlers[$identifier];

  if (!empty($handler['file'])) {
    $file_path = (!empty($handler['file path']) ? $handler['file path'] : drupal_get_path('module', $handler['module']));
    $file_path = rtrim($file_path, '/') .'/';
    include_once($file_path . $handler['file']);
  }

  $output = '';

  // Explicitly not checking the function's existance to allow debugging.
  if (!empty($handler['handler'])) {
    $args = array($node, $handler);
    $output .= call_user_func_array($handler['handler'], $args);
  }
  elseif (!empty($handler['form_handler'])) {
    $output .= drupal_get_form($handler['form_handler'], $node, $handler);
  }

  return $output;
}

/**
 * Helper form for outputting an upload form.
 */
function richmedia_upload_form($options = array()) {

  $defaults = array(
    'action' => NULL,
    'class' => 'upload',
    'field' => 'upload',
    'input-label' => t('Select file'),
    'button-label' => t('Upload'),
    'progress' => array(
      'path' => '',
    ),
    'redirect' => url($_REQUEST['q'], array('absolute' => TRUE)),
  );

  $options += $defaults;
  $form['#action'] = $options['action'];
  $form['#cache'] = FALSE;
  $form['#attributes'] = array(
    'enctype' => 'multipart/form-data',
    'class' => $options['class'],
  );

  $form[$options['field']] = array(
    '#type' => 'file',
    '#name' => 'file',
    '#title' => $options['input-label'],
    '#prefix' => '<div class="'. $options['class'] .'-upload">',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $options['button-label'],
    '#suffix' => '</div>',
  );

  $form['redirect_uri'] = array(
    '#type' => 'hidden',
    '#value' => $options['redirect'],
  );

  $form['transcode'] = array(
    '#type' => 'hidden',
    '#value' => 1,
  );

  /*
  $form['mediafile_id'] = array(
    '#type' => 'hidden',
    '#value' => $options['mediafile_id'],
  );*/
  return $form;
}

/**
 * Theme function for rendering a list of available upload handlers when no choice
 * was made.
 */
function theme_richmedia_upload_choice($handlers, $path = '') {
  $output = '';

  if ($handlers) {
    $output .= '<dl class="node-richmedia-upload-list">';
    foreach ($handlers as $key => $handler) {
      $output .= '<dt class="handler-'. check_plain($key) .'">'. l($handler['name'], $path .'/'. $key) .'</dt>';
      $output .= '<dd>'. $handler['description'] .'</dd>'. "\n";
    }
    $output .= '</dl>';
  }
  else {
    $output .= '<p>'. t('No upload handlers can be found. Please check the <a href="!link_modules">Installed modules page</a> to see which modules should be enabled.', array(
      '!link_modules' => url('admin/build/modules'),
    )) .'</p>';
  }

  return $output;
}
