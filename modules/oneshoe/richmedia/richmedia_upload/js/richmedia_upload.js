/* $Id$ */
/**
 * @file
 * This file contains javascript to support the richmedia_upload.
 */
(function ($) {
	Drupal.behaviors.clickListener = function(context) {
		$("input:checkbox", context).click(function() {
      $('.still_creation_options').toggleClass('active').toggleClass('inactive');
		});
  };

  Drupal.behaviors.selectListener = function(context) {
		$("select", context).change(function() {
			$("select option:selected").each(function() {
        var selected = $(this).val();
				$("input:text", context).each(function() {
          $(this).parent().addClass('inactive').removeClass('active');
          if ($(this).hasClass(selected.toLowerCase())) {
            $(this).parent().addClass('active').removeClass('inactive');
          }
				});
      });
    }).trigger('change');
	};

})(jQuery);