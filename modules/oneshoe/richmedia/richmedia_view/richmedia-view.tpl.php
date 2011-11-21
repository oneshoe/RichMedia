<?php
// $Id$
?>

<div class="richmedia">
<?php
if ($node->richmedia->count_video) :
  ?>
<div class="mediaplayers"><?php
  foreach ($node->richmedia->mediafiles as $mediafile) {
    print richmedia_get_mediaplayer($node->richmedia->asset_id, $mediafile, 450, 280);//widthXheight = 450x280
  }
  ?>
</div>
<?php
  endif;
if ($node->richmedia->count_still) :
  ?>
<div class="slides">
<?php
  for ($x = 0; $x < $node->richmedia->count_still; $x++) {
    ?>
<span class="slide"> <span class="time"><?php
    print $times[$x];
    ?></span>
<?php
    print theme('richmedia_view_still', 'overview_slide', $node, $x + 1);
    ?>
</span>
<?php
  }
  ?>
</div>

<?php endif;
?>
</div>
