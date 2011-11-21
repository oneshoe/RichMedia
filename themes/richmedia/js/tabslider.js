// $Id$

(function ($) {

Drupal.tabSlider = function(area) {
  var self = this;
  this.area = area;
  this.items = $('.item', this.area).length;
  this.currentItem = 0;
  this.currentItemId = '';
  this.sliderWidth = $(this.area).width() + 9;

  $('.slidepager-prev', this.area).click(function() { this.blur(); self.sliderGo(-1); return false });
  $('.slidepager-next', this.area).click(function() { this.blur(); self.sliderGo(1); return false });

  // Update the page numbering.
  $('.slidepager-pages', this.area).html(this.items);
}

Drupal.tabSlider.prototype.sliderGo = function(steps) {
  if (!steps) steps = 1;
  var dir = (steps < 0 ? 'ltr' : 'rtl');
  var gt = this.currentItem + steps;
  if (gt > this.items - 1) {
    return false;
  }
  else if (gt < 0) {
    return false;
  }

  var stagingLeft = (dir == 'ltr' ? '-' : '')+ this.sliderWidth +'px';
  var moveToLeft = (dir == 'ltr' ? '' : '-')+ this.sliderWidth +'px';

  $('.item.active-item', this.area).animate({
   left: moveToLeft,
   opacity: .2
  }, 400, 'swing', function() {
    $(this).removeClass('active-item').hide();
  }).css({position: 'absolute', top: '6px'});

  this.currentItemId = '#' + $('.item:eq('+ gt +')', this.area).show().css({left: stagingLeft, position: 'relative', top: '0px', opacity: .5}).addClass('active-item').animate({
    left: '9px',
    opacity: 1
  }, 450).attr('id');
  this.currentItem = gt;
  this.area.currentItemId = this.currentItemId;
  $('.slidepager-page', this.area).html(this.currentItem + 1);

  this.updatePager();

  // Return FALSE to cancel the default behaviour (fallback).
  return false;
};

Drupal.tabSlider.prototype.updatePager = function() {
  if (this.items == 0) {
    $('.slidepager', this.area).hide();
  }
  if (this.currentItem == 0) {
    $('.slidepager .slidepager-prev', this.area).addClass('disabled')//.attr('disabled', 'disabled');
  }
  else {
    $('.slidepager .slidepager-prev', this.area).removeClass('disabled')//.removeAttr('disabled');
  }

  if (this.currentItem == this.items - 1) {
    $('.slidepager .slidepager-next', this.area).addClass('disabled')//.attr('disabled', 'disabled');
  }
  else {
    $('.slidepager .slidepager-next', this.area).removeClass('disabled')//.removeAttr('disabled');
  }
}

Drupal.behaviors.tabSlider = function(context) {
  $('.tabslider:not(.tabslider-processed)', context).each(function() {
    this.tabSlider = new Drupal.tabSlider(this);
  });
};

})(jQuery);