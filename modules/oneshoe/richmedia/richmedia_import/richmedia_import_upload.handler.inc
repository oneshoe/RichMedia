<?php
// $Id$

/**
 * @file
 * File containing upload handling functions and form functionality.
 */

/**
 * Handler function for the upload forms.
 */
function richmedia_import_handler_upload_page($node = NULL, $handler = NULL) {
  return drupal_get_form('richmedia_import_upload_form', $node);
}


/**
 * Implementation of hook_form().
 */
function richmedia_import_upload_form($form_state, $node) {
  $form['#attributes'] = array(
    'enctype' => 'multipart/form-data',
    'class' => 'richmedia-import',
  );

  $form['upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload exportfile'),
    '#size' => 40,
    '#prefix' => '<div class="richmedia-import-upload">',
  );
  $form['attach'] = array(
    '#type' => 'submit',
    '#value' => t('Attach'),
    '#name' => 'attach',
    '#suffix' => '</div>',
  );

  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  return $form;
}

/**
 * Implementation of hook_submit().
 * @todo Use the correct path to move file to, use variable_get based on the admin settings.
 * @todo Checking of max_file_size against PHP settings. To ensure people dont upload and get error (or worse. nothing).
 *
 */
function richmedia_import_upload_form_submit(&$form, &$form_state) {
  $node = $form_state['values']['node'];
  $upload_path = variable_get('richmedia_import_uploadpath', file_directory_path().'/uploaded');

  // To be sure the directory is there
  file_check_directory($upload_path, FILE_CREATE_DIRECTORY);

  if (isset($_FILES['files']) && $_FILES['files']['name']['upload'] && is_uploaded_file($_FILES['files']['tmp_name']['upload'])) {
    // Check for file upload errors and return FALSE if a
    // lower level system error occurred.
    switch ($_FILES['files']['error']['upload']) {
      // @see http://php.net/manual/en/features.file-upload.errors.php
      case UPLOAD_ERR_OK:
        break;

      case UPLOAD_ERR_INI_SIZE:
      case UPLOAD_ERR_FORM_SIZE:
        form_set_error('upload', t('The file %file could not be saved, because it exceeds %maxsize, the maximum allowed size for uploads.', array('%file' => 'upload', '%maxsize' => format_size(file_upload_max_size()))));
        return 0;

      case UPLOAD_ERR_PARTIAL:
      case UPLOAD_ERR_NO_FILE:
        form_set_error('upload', t('The file %file could not be saved, because the upload did not complete.', array('%file' => 'upload')));
        return 0;

        // Unknown error
      default:
        form_set_error('upload', t('The file %file could not be saved. An unknown error has occurred.', array('%file' => 'upload')));
        return 0;
    }

    // Build the list of non-munged extensions.
    $extensions = '';
    $supported = array_keys(variable_get('richmedia_import_supported', array()));
    $filename = trim(basename($_FILES['files']['name']['upload']), '.');

    // Since we are working with archives, we have to check if the archive isn't tar.gz
    // If tar.gz is found, we use that as extension else we pop it from the filename.
    if (preg_match('/\.tar\.(gz|bz2)$/i', $filename, $matched)) {
      $final_extension = 'tar.'. $matched[1];
    }
    else {
      // Split the filename up by periods. The first part becomes the basename
      // the last part the final extension.
      $filename_parts = explode('.', $filename);
      $final_extension = array_pop($filename_parts); // Remove final extension.
    }
    $extensions = implode(" ", $supported);

    if (!in_array($final_extension, $supported)) {
      form_set_error('upload', t('File upload error. Extension not supported'));
      watchdog('file', 'Upload error. The Extension of %file is not supported.', array('%file' => $filename));
      return 0;
    }

    // Begin building file object.
    $file = new stdClass();
    $file->nid = $node->nid;
    $file->filename = file_munge_filename($filename, $extensions);
    $file->filepath = $_FILES['files']['tmp_name']['upload'];
    //$file->filemime = file_get_mimetype($file->filename);
    $file->source = 'upload';
    $file->destination = file_destination(file_create_path($upload_path .'/'. $file->filename), 0);
    $file->filesize = $_FILES['files']['size']['upload'];
    $file->fileext = $final_extension;

    // Move uploaded files from PHP's upload_tmp_dir to Drupal's temporary directory.
    // This overcomes open_basedir restrictions for future file operations.
    $file->filepath = $file->destination;
    if (!move_uploaded_file($_FILES['files']['tmp_name']['upload'], $file->filepath)) {
      form_set_error('upload', t('File upload error. Could not move uploaded file.'));
      watchdog('file', 'Upload error. Could not move uploaded file %file to destination %destination.', array('%file' => $file->filename, '%destination' => $file->filepath));
      return 0;
    }

    // If we made it this far it's safe to record this file in the database.
    drupal_write_record('richmedia_import', $file);
    _richmedia_asset_store($node->nid, $node->richmedia->asset_id, RICHMEDIA_STATUS_UPLOADED);
    drupal_set_message(t('File Successfully uploaded.'), 'status');
    $return = 'node/'. $node->nid;
  }
  else {
    drupal_set_message(t('Error during uploading the file, please try again.'), 'error');
    $return = implode('/', arg());
  }
  $form_state['redirect'] = $return;
}