<?php
// $Id: block.tpl.php 8 2009-09-15 14:55:49Z George $
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">
  <?php if (!empty($block->subject)): ?>
    <h3 class="block-title"><?php print $block->subject ?></h3>
  <?php endif;?>
  <div class="block-content-holder-bottom">
    <div class="block-content-holder-top">
      <div class="content">
        <?php print $block->content; ?>
      </div>
    </div>
  </div>
</div>
