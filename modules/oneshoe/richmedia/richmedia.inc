<?php
// $Id$

/**
 * @file
 * File containing API-like functions.
 */

/**
 * API function for starting a VP-X connection using the vpx_connector class.
 */
function richmedia_api_connect($user_id = NULL) {
  static $vpx = NULL;
  if (!isset($vpx)) {
    $_user_id = ($user_id ? $user_id : variable_get('vpx_connector_username', NULL));
    $_user_group = variable_get('richmedia_asset_group', 'richmedia');
    if ($_user_id && $_user_group) {
      if ($vpx = new vpx_connector($_user_id, $_user_group)) {
        richmedia_debug('Connected to VP-Core as user %user with group %group.', array(
          '%user' => $_user_id,
          '%group' => $_user_group,
        ), $vpx);
      }
      else {
        drupal_set_message(t('Error connecting to VP-Core.'), 'error');
        richmedia_debug('Error connecting to VP-Core as user %user with group %group.', array(
          '%user' => $_user_id,
          '%group' => $_user_group,
        ), (isset($_SESSION['vpx_connector_debug_info']) ? $_SESSION['vpx_connector_debug_info'] : NULL));
      }
    }
    else {
      watchdog('API CONNECT', t('Incorrect username: @username and/or usergroup: @usergroup', array('@username' => $_user_id, '@usergroup' => $_user_group)), WATCHDOG_ERROR);
    }
  }
  return $vpx;
}

/**
 * API function for creating an asset.
 * @param int $nid Node ID to place in 'reference_id' field.
 * @param int $user_id User identifier.
 */
function richmedia_api_create_asset($nid, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $response = $vpx->request('POST', '/asset/create', array(
      'user_id' => $user_id,
      'group_id' => variable_get('richmedia_asset_group', 'richmedia'),
      'reference_id' => $nid,
    ));
    if ($vpx->check_result($response)) {
      $asset_id = (string)$response->items->item->asset_id;
      richmedia_debug('Created new VP-Core asset %asset_id.', array('%asset_id' => $asset_id), $response);
      return $asset_id;
    }
  }
  return FALSE;
}

/**
 * API function for creating an empty mediafile within an asset.
 */
function richmedia_api_create_mediafile($asset_id, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $response = $vpx->request('POST', '/mediafile/create', array(
      'asset_id' => $asset_id,
      'user_id' => $user_id,
      'group_id' => variable_get('richmedia_asset_group', 'richmedia'),
    ));
    if ($vpx->check_result($response)) {
      $mediafile_id = (string)$response->items->item->mediafile_id;
      richmedia_debug('Created new VP-Core mediafile %mediafile_id for asset %asset_id.', array('%asset_id' => $asset_id, '%mediafile_id' => $mediafile_id), $response);
      return $mediafile_id;
    }
  }
  return FALSE;
}

/**
 * API function for creating an empty mediafile within an asset.
 */
function richmedia_api_create_still($mediafile_id, $user_id, $options = array()) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/mediafile/%s/still/create', $mediafile_id);

    $response = $vpx->request('POST', $url, $options);
    if ($vpx->check_result($response)) {
      $mediafile_id = (string)$response->items->item->mediafile_id;
      richmedia_debug('Created still job for %mediafile_id', array('%mediafile_id' => $mediafile_id), $response);
      return $mediafile_id;
    }
  }
  return FALSE;
}

/**
 * API function for deleting an asset. Usually, an asset can only be deleted when
 * there are no mediafiles or stills attached to it. By passing the optional
 * $recursive option you can delete the asset INCLUDING all relations to
 * mediafiles, stills or collections.
 * @param string $asset_id The VP-Core asset identifier.
 * @param string $user_id The User ID.
 * @param boolean $recursive Optional recursive parameter.
 * @return Returns FALSE on failure.
 */
function richmedia_api_delete_asset($asset_id, $user_id, $recursive = FALSE) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/asset/%s/delete', $asset_id);

    // Check if the user also wants to remove all mediafiles from the wle_asset.
    if ($recursive) {
      $url .= '?delete=cascade';
    }

    // Perform the delete request.
    $response = $vpx->request('POST', $url);
    richmedia_debug('Delete call to VP-Core @url.', array(
      '@url' => $url,
    ), $response);

    if ($vpx->check_result($response) === TRUE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Function to retrieve the information about an mediafile
 *
 */
function richmedia_api_get_mediafile($mediafile_id, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/mediafile/%s', $mediafile_id);
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}


/**
 * Function to play a file
 */
function richmedia_api_play_mediafile($asset_id, $mediafile_id, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/asset/%s/?mediafile_id=%s', $asset_id, $mediafile_id);
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_get_transcode_profiles($user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = '/transcode/profiles';
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_get_still($asset_id, $user_id, $options) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = richmedia_create_url('/asset/'. $asset_id .'/still', $options, 'url');
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );
    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_get_transcode_joblist($asset_id, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/asset/%s/joblist', $asset_id);
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_create_transcode_job($mediafile_id, $user_id, $options = array()) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = richmedia_create_url('/mediafile/'. $mediafile_id .'/transcode', $options);
    $response = $vpx->request('POST', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_asset_list($user_id, $limit = 100) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/asset?limit=%d&order_by=created&order_direction=desc', $limit);
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($response && $vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_create_upload_ticket($mediafile_id, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/mediafile/%s/uploadticket/create', $mediafile_id);
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($response && $vpx->check_result($response)) {
      $object = (object) array(
        'action' => (string) $response->items->item->action,
        'upload_progress' => (string) $response->items->item->uploadprogress_url,
        'ticket' => array_pop(explode("=", (string) $response->items->item->action)),
      );
      richmedia_debug('Created new VP-Core upload ticket %upload_ticket.', array('%upload_ticket' => $object->ticket), $response);
      return $object;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_asset_info($asset_id, $user_id) {
  if ($vpx = richmedia_api_connect($user_id)) {
    $url = sprintf('/asset/%s', $asset_id);
    $response = $vpx->request('GET', $url);
    richmedia_debug(
      'Request Media file to VP-Core @url',
      array(
        '@url' => $url,
      ),
      $response
    );

    if ($vpx->check_result($response) === TRUE) {
      return $response;
    }
  }
  return FALSE;
}

/**
 *
 */
function richmedia_api_play_call($asset_id, $user_id, $options) {
  if ($vpx = richmedia_api_connect($user_id)) {
    if (is_array($options) && (isset($options['mediafile_id']) || isset($options['profile_id']))) {
      $options['response'] = isset($options['response']) ? $options['response'] : 'plain';
      $url = richmedia_create_url('/asset/'. $asset_id .'/play', $options, 'url');
      $response = $vpx->request('GET', $url);
      richmedia_debug(
        'Request Media file to VP-Core @url',
        array(
          '@url' => $url,
        ),
        $response
      );
      if ($vpx->check_result($response) === TRUE) {
        return $response;
      }
    }
  }
  return FALSE;
}

/**
 * Create the url string needed for API functions.
 * This function seems to be overkill atm, only 1 function uses it but
 * it was created in advance so we can make all function abit more flexibel with the api.
 *
 * Extra parameters will be quite easy to add with this function.
 */
function richmedia_create_url($base_url, $arguments, $format = 'url') {
  //watchdog('richmedia_create_url', 'inputted info: '.dprint_r(array('base_url' => $base_url, 'arguments' => $arguments, 'format' => $format), TRUE));
  // Set Default values
  $extra = array();
  $replacements = array();

  if (!is_array($arguments)) {
    watchdog('Richmedia_create_url', "Given Default values isn't an array, current value is: ". print_r($arguments, TRUE));
    return FALSE;
  }

  foreach($arguments as $key => $value) {
    $extra[] = $key .'=%'.$key;
    $replacements['%'.$key] = $value;
  }

  if (count($replacements)) {
    $base_url .= '?'. implode('&', $extra);
  }

  switch($format) {
    case 'array':
      return array('url' => $base_url, 'arguments' => $replacements);
    break;
    case 'url':
    default:
      return strtr($base_url, $replacements);
    break;
  }
  return FALSE;
}