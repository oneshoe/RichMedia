// $Id$

(function ($) {

Drupal.behaviors.drupalTabs = function(context) {
  $('.drupaltabs:not(.drupaltabs-processed)', context).each(function() {
    var tabparent = $(this);

    $('.tabs li a', tabparent).each(function() {
      var hash = '', href = $(this).attr('href');
      if (href.indexOf('#') != -1) {
        hash = href.substring(href.indexOf('#') + 1, href.length);
      }
      if (hash && $('#'. hash)) {
        $(this).click(function() {
          $(this).blur();
          $('.tabs li.active', tabparent).removeClass('active');
          $('.tab-content.active-tab-content', tabparent).removeClass('active-tab-content').hide();

          $(this).parents('li').addClass('active');
          $('#'+ hash, tabparent).addClass('active-tab-content').hide().fadeIn(400);
          return false;
        });
      }
    });

    if ($('.tabs li.active', tabparent).length == 0) {
      $('.tabs li:first', tabparent).addClass('active').find('a').click();
    }
    tabparent.addClass('drupaltabs-processed');
  });
};

})(jQuery);