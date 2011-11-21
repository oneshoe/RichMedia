<?php // $Id: page.tpl.php 8 2009-07-23 17:00:49 George $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php  print $styles; ?>
    <!--[if IE 6]><link rel="stylesheet" type="text/css" media="screen" href="<?php print $base_path . $directory; ?>/css_core/ie6.css" /><![endif]-->
    <!--[if IE 7]><link rel="stylesheet" type="text/css" media="screen" href="<?php print $base_path . $directory; ?>/css_core/ie7.css" /><![endif]-->
    <?php print $scripts; ?>
  </head>
  <body class="<?php print $body_classes; ?>">
    <div id="header">
      <div class="header-content">
        <?php if ($site_name): ?>
          <h1 id="site-name"><a href="<?php print $front_page; ?>" title="<?php print t('home'); ?>"><?php  print $site_name ?></a></h1>
        <?php endif; ?>
        <?php if ($logo): ?>
          <span class="logo"><a href="<?php print $front_page; ?>" title="<?php print t('home'); ?>"><?php print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo" />';?></a></span>
        <?php endif; ?>

        <div id="menu-holder">
          <?php if (isset($primary_links)) : ?>
            <div id="main-menu">
              <?php print theme('links', $primary_links, array('class' => 'menu primary-links')) ?>
            </div>
          <?php endif; ?>
          <div class="user-menu">
            <?php  print $menu; ?>
          </div>
        </div>
      </div>
    </div>
    <div id="container-holder">
      <div id="container">
        <?php if (isset($secondary_links)) : ?>
          <?php if ($logged_in): ?>
            <div id="submenu">
              <?php print theme('links', $secondary_links, array('class' => 'menu secondary-links')); ?>
            </div>
          <?php endif; ?>
        <?php endif; ?>
        <div id="main-region">
          <?php if ($title): print '<h2 class="page-title">'. $title .'</h2>'; endif; ?>
          <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
          <?php if ($tabs): print $tabs .'</div>'; endif; ?>
          <?php if ($tabs2): print $tabs2; endif; ?>
          <?php if ($show_messages && $messages): print $messages; endif; ?>
          <?php print $help; ?>
          <div class="content-page">
            <?php print $content ?>
          </div>
        </div>
        <?php if ($right): ?>
          <div id="sidebar-right">
            <?php  print $right ?>
          </div>
        <?php endif; ?>

      </div>
    </div>

    <div id="footer">
      <div class="footer-message"><?php print $footer_message; ?></div>
      <div id="footer-menu">
        <?php print $footer; ?>
      </div>
    </div>
    <?php print $closure ?>
  </body>
</html>
