<?php
// $Id$

/**
 * Implements hook_profile_details().
 *
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *  An array with keys 'name' and 'description' describing this profile.
 */
function surfnet_richmedia_profile_details() {
  return array(
    'name'        => 'SURFnet Rich Media PoC',
    'description' => 'This install profile installs modules needed for the proof-of-concept for Rich Media presentations for SURFnet.',
  );
}

/**
 * Implements hook_profile_modules().
 */
function surfnet_richmedia_profile_modules() {

  // Added a small hack to avoid any time-limit problems on slower machines.
  // Allow the install process to take up 2 minutes of extra time.
  @set_time_limit(180);

  return array(
    // Core optional modules.
    'menu', 'path', 'dblog', 'help', 'taxonomy', 'search',

    // Contrib modules.
    'content', 'date_api', 'date_timezone', 'date',
    'imageapi', 'imageapi_gd', 'imagecache',
    'tagadelic',
    'views',

    // Rich Media modules
    'vpx_connector', 'richmedia', 'richmedia_upload', 'richmedia_import', 'richmedia_view',

    // @todo Remove these, these are only temporary.
    'oneshoe_drupal_reinstall', 'views_ui',
  );
}

/**
 * Implements hook_profile_tasks().
 *
 * Perform standard tasks.
 */
function surfnet_richmedia_profile_tasks(&$task, $url) {
  if ($task == 'profile') {

    // Insert default user-defined node types into the database.
    $types = array(
      array(
        'type' => 'presentation',
        'name' => st('Presentation'),
        'module' => 'node',
        'description' => st("Use this type of content for uploaded presentations."),
        'custom' => TRUE,
        'modified' => TRUE,
        'locked' => FALSE,
        'help' => '',
        'min_word_count' => '',
      ),
    );

    foreach ($types as $type) {
      $type = (object) _node_type_set_defaults($type);
      node_type_save($type);
    }

    // Set default variables here.
    _surfnet_richmedia_profile_set_variables(array(

      // Default page to not be promoted and have comments disabled.
      'node_options_presentation' => array('status'),
      'comment_presentation' => 0,

      // Settings for Rich Media on the presentation content type.
      'richmedia_presentation' => TRUE,
      'richmedia_upload_enabled_presentation' => TRUE,
      'richmedia_upload_enabled' => TRUE,
      'richmedia_view_garbage_time' => 86400,
      'richmedia_import_uploadpath' => 'sites/default/files/uploaded',
      'richmedia_import_unpackpath' => '/tmp/richmedia/unpack',
      'richmedia_import_default_transcode_profile' => 1,
      'richmedia_asset_create' => TRUE,
      'richmedia_asset_delete' => 2,
      'richmedia_asset_group' => 'richmedia_group',
      'richmedia_debug' => array(1 => 1),

      // Performance settings.
      'cache' => 1,
      'preprocess_css' => 1,
      'preprocess_js' => 1,

      // Tagadelic settings.
      'tagadelic_sort_order' => 'title,asc',
      'tagadelic_page_amount' => 100,
      'tagadelic_levels' => 6,

      // Set "Only site administrators can create new user accounts"
      'user_register' => 0,

      // Date configuration.
      'configurable_timezones' => 0,
      'date_first_day' => 1,
      'date_default_timezone_name' => 'Europe/Amsterdam',

      // Set the default theme and admin theme.
      'theme_default' => 'richmedia',
      'admin_theme' => 'garland',
    ));

    // Create vocabularies.
    _surfnet_richmedia_profile_vocabularies();

    // Add fields to content types.
    _surfnet_richmedia_profile_create_cck_fields();

    // Enable the richmedia theme.
    _surfnet_richmedia_profile_enable_theme('richmedia');

    // Install the default views.
    _surfnet_richmedia_profile_install_default_views();

    // Create menu items.
    _surfnet_richmedia_profile_build_menu();
  }
}

/**
 * Helper function for mass-setting variables.
 * @param array $vars
 *   Provide an array keyed by the variable name and with the value as default
 *   value.
 */
function _surfnet_richmedia_profile_set_variables($vars) {
  foreach ($vars as $key => $value) {
    variable_set($key, $value);
  }
}

function _surfnet_richmedia_profile_ensure_block($module, $delta, $theme, $region = 'left', $weight = 1, $visibility = 0, $pages = '') {
  if ($bid = db_result(db_query("SELECT bid FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme))) {
    db_query("UPDATE {blocks} SET region = '%s', weight = %d, status = %d, visibility = %d, pages = '%s' WHERE bid = %d", $region, $weight, 1, $visibility, $pages, $bid);
  }
  else {
    $block = array('module' => $module, 'delta' => $delta, 'theme' => $theme, 'weight' => $weight, 'region' => $region, 'status' => 1, 'custom' => 0, 'throttle' => 0, 'visibility' => $visibility, 'cache' => -1, 'pages' => $pages);
    drupal_write_record('blocks', $block);
  }
}

/**
 * Enable a theme.
 *
 * @param $theme
 *   Unique string that is the name of theme.
 */
function _surfnet_richmedia_profile_enable_theme($theme) {
  db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' AND name = '%s'", $theme);
  system_initialize_theme_blocks($theme);
}

/**
 * Disable a theme.
 *
 * @param $theme
 *   Unique string that is the name of theme.
 */
function _surfnet_richmedia_profile_disable_theme($theme) {
  db_query("UPDATE {system} SET status = 0 WHERE type = 'theme' AND name = '%s'", $theme);
  system_theme_data();
}

/**
 * Create instances of CCK fields and attach them to a content type.
 */
function _surfnet_richmedia_profile_create_cck_fields() {
  $fields = array();
  $fields['field_presentation_date'] = array (
    'label' => 'Presentation date',
    'field_name' => 'field_presentation_date',
    'type' => 'date',
    'widget_type' => 'date_select',
    'weight' => '-4',
    'default_value' => 'blank',
    'default_value2' => 'same',
    'default_value_code' => '',
    'default_value_code2' => '',
    'input_format' => 'd/m/Y - H:i:s',
    'input_format_custom' => '',
    'year_range' => '-3:+3',
    'increment' => '1',
    'advanced' => array(
      'label_position' => 'above',
      'text_parts' => array(
        'year' => 0,
        'month' => 0,
        'day' => 0,
        'hour' => 0,
        'minute' => 0,
        'second' => 0,
      ),
    ),
    'label_position' => 'above',
    'text_parts' => array(),
    'description' => '',
    'required' => 1,
    'multiple' => '0',
    'repeat' => 0,
    'todate' => '',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
    ),
    'default_format' => 'custom',
    'tz_handling' => 'none',
    'timezone_db' => '',
    'module' => 'date',
    'widget_module' => 'date',
    'columns' => array(
      'value' => array(
        'type' => 'varchar',
        'length' => 20,
        'not null' => false,
        'sortable' => true,
        'views' => true,
      ),
    ),
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'custom',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'custom',
        'exclude' => 1,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'type_name' => 'presentation',
  );

  module_load_include('inc', 'content', 'includes/content.crud');
  foreach ($fields as $key => $field) {
    content_field_instance_create($field);
  }
}

/**
 * Create Taxonomy vocabularies.
 */
function _surfnet_richmedia_profile_vocabularies() {
  $default = array(
    'tags' => 1,
    'hierarchy' => 0,
    'relations' => 0,
    'multiple' => 1,
    'nodes' => array('presentation' => 1),
  );

  $vocabularies = array(
    'presenter' => array(
      'name' => 'Presenter',
      'required' => 1,
      'weight' => 0,
    ) + $default,
    'tags' => array(
      'name' => 'Tags',
      'weight' => 1,
    ) + $default,
  );

  foreach ($vocabularies as $key => &$vocabulary) {
    taxonomy_save_vocabulary($vocabulary);
  }
}

/**
 * Build menu items.
 */
function _surfnet_richmedia_profile_build_menu() {
  $items = array(
    '<front>' => array(
      'link_path' => '<front>',
      'weight' => -10,
      'link_title' => 'Homepage',
      'menu_name' => 'primary-links',
    ),
    'node/add/presentation' => array(
      'link_path' => 'node/add/presentation',
      'link_title' => 'Add presentation',
      'weight' => 1,
      'menu_name' => 'primary-links',
    ),
    'search' => array(
      'link_path' => 'search',
      'link_title' => 'Search',
      'weight' => 2,
      'menu_name' => 'primary-links',
    ),
  );

  $menu_names = array();
  $parents = array();
  foreach ($items as $path => &$item) {
    if (!isset($item['link_path'])) $item['link_path'] = $path;

    if (isset($item['_parent'], $parents[$item['_parent']])) {
      $item['plid'] = $parents[$item['_parent']];
      unset($item['_parent']);
    }

    $mlid = menu_link_save($item);
    $parents[$path] = $mlid;
    $menu_names[$item['menu_name']] = $item['menu_name'];
  }

  foreach ($menu_names as $menu_name) {
    menu_cache_clear($menu_name);
  }
}

/**
 * Installs the default views.
 */
function _surfnet_richmedia_profile_install_default_views() {
  include_once 'surfnet_richmedia.views.inc';
  views_include('view');

  $views = _surfnet_richmedia_profile_default_views();

  foreach ($views as $view) {
    $view->save();
  }

  // Clear the views caches.
  views_invalidate_cache();
  cache_clear_all();
}
