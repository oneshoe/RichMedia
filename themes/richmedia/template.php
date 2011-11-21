<?php
// $Id$

// include custom pager
include_once('template-pager.php');



// Unset the default drupal css
function richmedia_preprocess_page(&$vars) {
  $css = $vars['css'];
  unset($css['all']['module']['modules/node/node.css']);
  unset($css['all']['module']['modules/system/system-menus.css']);
  unset($css['all']['module']['modules/system/system.css']);
  unset($css['all']['module']['modules/system/defaults.css']);
  unset($css['all']['module']['modules/user/user.css']);
  unset($css['all']['module']['sites/all/modules/cck/theme/content.css']);
  $vars['styles'] = drupal_get_css($css);
}

/**
 * Override the default richmedia_view theme. We use node-presentation.tpl.php
 * @param object $node
 */
function richmedia_preprocess_node($node) {
  // Add the interface javascript.
drupal_add_js(drupal_get_path('theme', 'richmedia') . '/js/tabslider.js', 'theme');
drupal_add_js(drupal_get_path('theme', 'richmedia') .'/js/drupaltabs.js', 'theme');
drupal_add_js(drupal_get_path('theme', 'richmedia') .'/js/richmedia.js', 'theme');

}

function old_richmedia_preprocess_richmedia_view(&$vars) {
    // Add the interface javascript.
    drupal_add_js(path_to_theme() .'/js/tabslider.js', 'theme');
    drupal_add_js(path_to_theme() .'/js/drupaltabs.js', 'theme');
    drupal_add_js(path_to_theme() .'/js/richmedia.js', 'theme');

    // Do the actual callback to the server.
    module_load_include('inc', 'richmedia');
    $user_id = variable_get('vpx_connector_username', NULL);

    // Set default values.
    $players = array();
    $namelist = array('primary', 'secondary', 'tertiary', 'quaternary');
    $path = base_path().path_to_theme() .'/jw/';
    $i = 1;

    if (isset($vars['richmedia']->mediafiles) && is_array($vars['richmedia']->mediafiles)) {
      foreach($vars['richmedia']->mediafiles as $key => $value) {
        $output = richmedia_api_play_call($vars['richmedia']->asset_id, $user_id, array('profile_id' => variable_get('richmedia_import_default_transcode_profile', 0), 'original_mediafile_id' => $vars['richmedia']->mediafiles[0]));
        $callback = (string) $output->items->item->output;
        $info = richmedia_timecode_array($vars['node']->nid);
        $players[$namelist[$key]] = array('callback' => $callback, 'timecodes' => $info);
      }
    }
    if (count($players)) {
      foreach($players as $name => $settings) {
        $info = array();
        if ($i <= 1) {
          $info = array (
            'slides' => array (
              'selector' => '#tab-slides-detail',
              'timecodes' => $settings['timecodes'],
            ),
          );
        }

        // Load the LongTailVideo player scripts.
        $vars['player_'.$name] = richmedia_mediaplayer(
          $info,
          array(
            'file' => $settings['callback'],
            'config' => $path .'config.xml',
            'id' => 'player_'.$i,
          ),
          450,
          280,
          '9.0.98'
        );
        $i++;
      }
    } else {
      $vars['player_primary'] = t('No mediafiles for asset !asset', array('!asset' => $vars['richmedia']->asset_id));
    }
  }



/**
 * Function for loading the JWplayer
 */
function richmedia_mediaplayer($data, $flashvars, $width, $height, $version = FALSE, $default_content = '') {
  static $longtail_index, $longtail_version;

  $player_swf = path_to_theme() .'/jw/player-licensed.swf';
  $version = ($version ? $version : '9.0.98');
  $default_content = ($default_content ? $default_content : t('To watch this video you need the Adobe&reg; Flash plugin. <a href="http://get.adobe.com/flashplayer">Click here</a> to download the Flash plugin.'));

  if (!isset($longtail_index)) {
    $longtail_index = 1;
    $longtail_version = filemtime($player_swf);
    drupal_add_js(path_to_theme() .'/jw/swfobject.js', 'theme', 'footer');
  }

  $player_id = 'player_'. $longtail_index;
  $player_swf .= '?v'.$longtail_version;

  // Add the swfObject call.
  $js = "// This is old swfobject implementation, the only one to correctly call playerLoaded() function.\n";
  $js .= "var so = new SWFObject('". base_path().$player_swf ."', '". $player_id ."', ". $width .", ". $height .", '". $version ."', '#ffffff'); ";
  $js .= "so.addParam('wmode','transparent');so.addParam('allowfullscreen','true');so.addParam('allowscriptaccess','always');\n";
  foreach ($flashvars as $name => $value) {
    $js .= "so.addVariable('". $name ."','". str_replace("'", "\\'", $value) ."'); ";
  }
  $js .= "so.write('". $player_id ."-container');";
  drupal_add_js($js, 'inline', 'footer');

  // Add the player setting.
  drupal_add_js(array('rm_player' => array($player_id => $data)), 'setting');

  // Add the placement div and the <noscript> version.
  $params = array(
    'movie' => base_path().$player_swf,
    'allowfullscreen' => 'true',
    'allowscriptaccess' => 'always',
    'flashvars' => drupal_query_string_encode($flashvars),
  );

  $param_str = '';
  foreach ($params as $name => $value) {
    $param_str .= "\n".'<param name="'. $name .'" value="'. check_plain($value) .'">';
  }

  $output = "<div class=\"player-hasjs\" id=\"". $player_id ."-container\"><p>". $default_content ."</p></div>";

  $output .= '<div class="player-nojs"><object id="'. $player_id .'-alt" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" name="'. $player_id .'" width="'. $width .'" height="'. $height .'">';
  $output .= $param_str;
  $output .= '<object type="application/x-shockwave-flash" data="'. base_path().$player_swf .'" width="'. $width .'" height="'. $height .'">';
  $output .= $param_str;
  $output .= '<p>'. $default_content .'</p>';
  $output .= '</object>';
  $output .= '</object></div>';

  $longtail_index++;

  return $output;
}

/**
 *
 */
function richmedia_form($element) {
  $prefix = $suffix = '';
  switch($element['#id']) {
    case 'user-login':
    case 'user-register':
    case 'user-profile-form':
    case 'user-pass':
    case 'node-form':
    case 'node-delete-confirm':
      $prefix = '<div id="form-holder">';
      $suffix = '</div>';
    break;
    default:
  }
  $action = $element['#action'] ? 'action="'. check_url($element['#action']) .'" ' : '';
  return $prefix .'<form '. $action .' accept-charset="UTF-8" method="'. $element['#method'] .'" id="'. $element['#id'] .'"'. drupal_attributes($element['#attributes']) .">\n<div>". $element['#children'] ."\n</div></form>$suffix\n";
}


/**
 * Function to generate the slides needed on the presentation page
 */
function richmedia_generate_slides($type, $node) {
  $output = '';
  switch($type) {
    case 'detail':
      for ($x = 1; $x < $node->richmedia->count_still; $x++) {
        $class = ($x === 1) ? 'item active-item' :  'item';
        $output .= '  <div id="item-'. $x .'" class="'. $class .'">';
        $output .= '    <div class="slide">';
        $output .= '      '. theme('richmedia_view_still', 'detail_slide', $node, $x);
        $output .= '    </div>';
        $output .= '  </div>';
      }
      $output .= '  <div class="slidepager">';
      $output .= '    <a class="slidepager-prev disabled" href="#" title="previous">previous </a>';
      $output .= '    <div class="page-holder">';
      $output .= '      <span class="slidepager-page slidepager-nr">1</span> of <span class="slidepager-pages slidepager-nr">'. $node->richmedia->count_still .'</span>';
      $output .= '    </div>';
      $output .= '    <a class="slidepager-next" href="#" title="previous">next </a>';
      $output .= '  </div>';
      break;
    case 'overview':
      $timing = $node->richmedia->still_timing;
      for ($x = 0; $x < $node->richmedia->count_still; $x++) {
        if ($x % 9 == 0) {
          if ($x != 0) {
            $output .= '  </div>';
          }
          $class = 'item';

          if ($x == 0) {
            $class = 'item active-item';
            $l = 1;
          }

          $output .= '  <div id="item-'. $l++ .'" class="'. $class .'">';
        }
        $output .= '    <div class="slide">';
        $output .= '      '. theme('richmedia_view_still', 'overview_slide', $node, $x+1);
        $output .= '      <span class="time">'. richmedia_view_parse_time($timing[$x]) .' - '. richmedia_view_parse_time($timing[$x+1]) .'</span>';
        $output .= '    </div>';
      }
      $output .= '  </div>';
      $output .= '  <div class="slidepager">';
      $output .= '    <a class="slidepager-prev disabled" href="#" title="previous">previous</a>';
      $output .= '    <div class="page-holder">';
      $output .= '      <span class="slidepager-page slidepager-nr">1</span> of <span class="slidepager-pages slidepager-nr">'. $l .'</span>';
      $output .= '    </div>';
      $output .= '    <a class="slidepager-next" href="#" title="previous">next</a>';
      $output .= '  </div>';
      break;
  }
  return $output;
}
