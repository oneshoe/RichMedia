<?php
// $Id: node.tpl.php 8 2009-05-12 11:55:49Z thijs $
?>

<?php
$node_terms = array();
$node_terms_links = array();
foreach ($node->taxonomy as $term) {
  $node_terms[$term->vid][$term->tid] = check_plain($term->name);
  $node_terms_links[$term->vid][$term->tid] = l($term->name, 'taxonomy/term/'. $term->tid);
}
?>
<?php if ($page == 0): ?>
  <div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">
    <div class="teaser-image">
      <a href="<?php print $node_url ?>" title="<?php print $title ?>"><img src="<?php print $base_path . $directory; ?>/temp/teaser_image.gif" alt="" /></a>
    </div>

    <div class="teaser-content content clear-block">
      <h3 class="teaser-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h3>
        <dl>
        <?php if ($node_terms[1]): ?>
          <dt><?php print t('Presenter(s)'); ?>:</dt>
            <dd><?php print implode(', ', $node_terms[1]); ?></dd>
        <?php endif; ?>
        <dt><?php print t('Date'); ?>:</dt>
          <dd><?php print $node->field_presentation_date[0]['view']; ?></dd>
        <?php if ($node_terms[2]): ?>
          <dt><?php print t('Tags'); ?>:</dt>
            <dd><?php print implode(', ', $node_terms[2]);
            ?></dd>
        <?php endif; ?>
      </dl>
    </div>
    <div class="clear-block"> </div>
  </div>

<?php else: ?>

  <div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

        <?php print $content ?>
        <div class="presentation-info">
          <dl>
            <?php if ($node_terms_links[1]): ?>
              <dt><?php print t('Presenter(s)'); ?>:</dt>
                <dd><?php print implode(', ', $node_terms_links[1]); ?></dd>
            <?php endif; ?>
             <dt><?php print t('Date'); ?>:</dt>
              <dd><?php print $node->field_presentation_date[0]['view']; ?></dd>
            <?php if ($node_terms_links[2]): ?>
              <dt><?php print t('Tags'); ?>:</dt>
                <dd><?php print implode(', ', $node_terms_links[2]);
                ?></dd>
            <?php endif; ?>
          </dl>
        </div>
      </div>
    </div>
  </div>

<?php endif; ?>