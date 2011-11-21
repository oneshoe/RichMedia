<?php
// $Id: node.tpl.php 8 2009-05-12 11:55:49Z thijs $
?>

<div class="node-left">
<div class="content clear-block">
<?php
print richmedia_mediaplayer(array(
  'slides' => array(
    'selector' => '#tab-slides-detail',
    'timecodes' => $node->richmedia->still_timing
  )
), array(
  'file' => $node->richmedia->videos[0],
  'config' => base_path() . path_to_theme() . '/jw/config.xml',
  'id' => 'player_1'
), 450, 280, '9.0.98');

if ($node->richmedia->count_still > 0 || $node->richmedia->count_video > 0) {
  ?>
  </div>
</div>
<div class="node-right drupaltabs">
<ul class="tabs">
        <?php
  if ($node->richmedia->count_still > 0) {
    ?>
        <li class="first active"><a href="#tab-slides-detail" class="">Slide</a></li>
	<li><a href="#tab-slides-overview" class="">Overview</a></li>
        <?php
  }
  ?>
        <?php
  if ($node->richmedia->count_video > 1) {
    ?>
        <li class="last"><a href="#tab-video" class="">Video</a></li>
        <?php
  }
  ?>
      </ul>
<div class="media-holder-top">
<div class="media-holder-footer">
<div class="tab-content" id="tab-video">
<div id="item-100" class="item active-item">
<div class="slide">
                <?php
  print richmedia_mediaplayer(array(
    'slides' => array(
      'selector' => '#tab-slides-detail',
      'timecodes' => $node->richmedia->still_timing
    )
  ), array(
    'file' => $node->richmedia->videos[1],
    'config' => base_path() . path_to_theme() . '/jw/config.xml',
    'id' => 'player_2'
  ), 450, 280, '9.0.98');
  ?>
              </div>
</div>
</div>
<div class="tab-content active-tab-content tabslider"
	id="tab-slides-detail">
            <?php
  print richmedia_generate_slides('detail', $node);
  ?>
          </div>
<div class="tab-content tabslider" id="tab-slides-overview">
            <?php
  print richmedia_generate_slides('overview', $node);
  ?>
          </div>
</div>
</div>
</div>
<?php
}
?>




