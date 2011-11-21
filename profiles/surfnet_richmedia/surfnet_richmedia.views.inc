<?php
// $Id$

/**
 * @file
 * This file contains default views to be installed using the install profile.
 */

/**
 * Returns some views exports.
 */
function _surfnet_richmedia_profile_default_views() {
  $views = array();

  /**
   * Featured view, for use in a block on the frontpage.
   */
  $view = new view;
  $view->name = 'featured';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'created' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'promote' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'promote',
      'table' => 'node',
      'field' => 'promote',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'presentation' => 'presentation',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 1);
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));
  $handler = $view->new_display('block', 'Featured block', 'block_1');
  $handler->override_option('title', 'Featured');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);

  $views['featured'] = $view;

  /**
   * Frontpage view.
   */
  $view = new view;
  $view->name = 'frontpage';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'field_presentation_date_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_presentation_date_value',
      'table' => 'node_data_field_presentation_date',
      'field' => 'field_presentation_date_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'presentation' => 'presentation',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('footer', '<a title="More presentations" href="' . base_path() . 'presentations">More presentations</a></p>');
  $handler->override_option('footer_format', '3');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('title', 'Recent presentations');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('path', 'node');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  $views['frontpage'] = $view;

  /**
   * Presentations overview.
   */
  $view = new view;
  $view->name = 'presentation';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'field_presentation_date_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_presentation_date_value',
      'table' => 'node_data_field_presentation_date',
      'field' => 'field_presentation_date_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'presentation' => 'presentation',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('empty', 'There are no presentations.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_pager', '1');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('title', 'Presentations');
  $handler->override_option('path', 'presentations');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'All presentations',
    'description' => '',
    'weight' => '0',
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'Presentations',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler = $view->new_display('page', 'Page', 'page_2');
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'presentation' => 'presentation',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'uid_current' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'uid_current',
      'table' => 'users',
      'field' => 'uid_current',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'My presentations');
  $handler->override_option('path', 'presentations/my');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'My presentations',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  $views['presentation'] = $view;

  /**
   * Presentation search.
   */
  $view = new view;
  $view->name = 'presentation_search';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'score' => array(
      'order' => 'DESC',
      'id' => 'score',
      'table' => 'search_index',
      'field' => 'score',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_presentation_date_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_presentation_date_value',
      'table' => 'node_data_field_presentation_date',
      'field' => 'field_presentation_date_value',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'presentation' => 'presentation',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'keys' => array(
      'operator' => 'required',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'keys_op',
        'identifier' => 'keys',
        'label' => 'Keywords',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'keys',
      'table' => 'search_index',
      'field' => 'keys',
      'relationship' => 'none',
    ),
    'tid' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'tid_op',
        'identifier' => 'presenter',
        'label' => 'Presenter',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'type' => 'select',
      'limit' => TRUE,
      'vid' => '1',
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'hierarchy' => 0,
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'date_filter' => array(
      'operator' => '=',
      'value' => array(
        'value' => NULL,
        'min' => NULL,
        'max' => NULL,
        'default_date' => '',
        'default_to_date' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'date_filter_op',
        'identifier' => 'date',
        'label' => 'Date',
        'optional' => 1,
        'remember' => 0,
      ),
      'date_fields' => array(
        'node_data_field_presentation_date.field_presentation_date_value' => 'node_data_field_presentation_date.field_presentation_date_value',
      ),
      'date_method' => 'OR',
      'granularity' => 'day',
      'form_type' => 'date_select',
      'default_date' => '',
      'default_to_date' => '',
      'year_range' => '-3:+3',
      'id' => 'date_filter',
      'table' => 'node',
      'field' => 'date_filter',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('empty', 'Your search yielded no results.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_pager', '1');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'build_mode' => 'teaser',
    'links' => 1,
    'comments' => 0,
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('title', 'Search presentations');
  $handler->override_option('path', 'search');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));

  $views['presentation_search'] = $view;


  /**
   * Taxonomy override view.
   */
  $view = new view;
  $view->name = 'taxonomy_term_presentations';
  $view->description = 'A view to emulate Drupal core\'s handling of taxonomy/term; it also emulates Views 1\'s handling by having two possible feeds.';
  $view->tag = 'default';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'sticky' => array(
      'id' => 'sticky',
      'table' => 'node',
      'field' => 'sticky',
      'order' => 'DESC',
      'relationship' => 'none',
    ),
    'created' => array(
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'order' => 'DESC',
      'granularity' => 'second',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'term_node_tid_depth' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(
        'count' => TRUE,
        'override' => FALSE,
        'items_per_page' => 25,
      ),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '%1',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'taxonomy_term',
      'validate_fail' => 'not found',
      'depth' => '0',
      'break_phrase' => 1,
      'set_breadcrumb' => 0,
      'id' => 'term_node_tid_depth',
      'table' => 'node',
      'field' => 'term_node_tid_depth',
      'relationship' => 'none',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'page' => 0,
        'presentation' => 0,
      ),
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '2' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
        '3' => 0,
        '4' => 0,
      ),
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
    ),
    'term_node_tid_depth_modifier' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(
        'count' => TRUE,
        'override' => FALSE,
        'items_per_page' => 25,
      ),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'id' => 'term_node_tid_depth_modifier',
      'table' => 'node',
      'field' => 'term_node_tid_depth_modifier',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
      'operator' => '=',
      'value' => '',
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('use_pager', '1');
  $handler->override_option('row_plugin', 'node');
  $handler->override_option('row_options', array(
    'teaser' => TRUE,
    'links' => TRUE,
  ));
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('arguments', array(
    'term_node_tid_depth' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(
        'count' => TRUE,
        'override' => FALSE,
        'items_per_page' => 25,
      ),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '%1',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'depth' => '0',
      'break_phrase' => 1,
      'set_breadcrumb' => 0,
      'id' => 'term_node_tid_depth',
      'table' => 'node',
      'field' => 'term_node_tid_depth',
      'relationship' => 'none',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'page' => 0,
        'presentation' => 0,
      ),
      'validate_argument_vocabulary' => array(
        '1' => 0,
        '2' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'term_node_tid_depth_modifier' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(
        'count' => TRUE,
        'override' => FALSE,
        'items_per_page' => 25,
      ),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Alle',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'id' => 'term_node_tid_depth_modifier',
      'table' => 'node',
      'field' => 'term_node_tid_depth_modifier',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('path', 'taxonomy/term/%');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('feed', 'Core feed', 'feed');
  $handler->override_option('items_per_page', 15);
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('row_options', array(
    'item_length' => 'default',
  ));
  $handler->override_option('path', 'taxonomy/term/%/%/feed');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array(
    'page' => 'page',
    'default' => 0,
  ));
  $handler->override_option('sitename_title', FALSE);
  $handler = $view->new_display('feed', 'Views feed', 'feed_1');
  $handler->override_option('items_per_page', 15);
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('row_options', array(
    'item_length' => 'default',
  ));
  $handler->override_option('path', 'taxonomy/term/%/feed');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array());
  $handler->override_option('sitename_title', FALSE);

  $views['taxonomy_term_presentations'] = $view;

  return $views;
}
