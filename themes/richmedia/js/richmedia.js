/* $Id$ */
/**
 * @file
 * This file contains the interface javascript for the rich media video playback.
 */

var rm_leadingPlayer = 0;
var rm_followingPlayer = 0;
var rm_players = {};
var currentTime = {};
var previousTime = {};
var previousState = {};
var videoCount = 0;

// This function will be called by the JW player.
function playerReady(player) {
  if (typeof(Drupal.settings.rm_player[player.id]) != undefined) {
    var pobj = document.getElementById(player.id);
    if (!pobj) {
      setTimeout(playerReady, 100, player);
      return;
    }

    if (typeof(Drupal.settings.rm_player[player.id].slides) != 'undefined') {
      pobj.currentSlideItem = 0;
      pobj.tabSlider = $(Drupal.settings.rm_player[player.id].slides.selector).get(0).tabSlider;
      pobj.info = {duration: 0, filesize: 0, loaded: 0};
    }
    rm_players[player.id] = pobj;
    rm_attachListeners(pobj, player.id);
    videoCount = objLength(rm_players);
  }
}

function rm_attachListeners(pobj, id) {
  if (typeof(Drupal.settings.rm_player[id].slides) != 'undefined') {
    pobj.addModelListener("TIME", "rm_slideListener");
    pobj.addModelListener("LOADED", "rm_loadListener");
  }
  pobj.addModelListener("TIME", "rm_syncListener");
  pobj.addModelListener("STATE", "rm_stateListener");
}

function rm_loadListener(player) {
  rm_players[player.id].info.filesize = player.total;
  rm_players[player.id].info.loaded = player.loaded;
}

function rm_slideListener(player) {
  if (previousTime[player.id] == undefined) {
    previousTime[player.id] = player.position;
  }
  currentTime[player.id] = player.position;
  if (player.position < 0.1 || player.position % 0.5 == 0) {
    var n = rm_retrieveCurrentSlideIndex(player);
    var c = rm_players[player.id].tabSlider.currentItem;
    rm_players[player.id].info.duration = player.duration;
    if (c != n) {
      var step = n - c;
      if (step != 0) {
        rm_players[player.id].tabSlider.sliderGo(step);
        rm_players[player.id].currentSlideItem = n;

        $('#tab-slides-overview .slide-active').removeClass('slide-active');
        $('#tab-slides-overview .slide:eq('+ n +')').addClass('slide-active');
      }
    }
  }
}

function rm_syncListener(player) {
  if (videoCount > 1) {
    //...player1 was changed, so sync player2
    if((currentTime[rm_leadingPlayer] < previousTime[rm_leadingPlayer]) || (currentTime[rm_leadingPlayer] > previousTime[rm_leadingPlayer] + .4)) {
      rm_players[rm_followingPlayer].sendEvent('SEEK', (currentTime[rm_leadingPlayer] - 0.2)); //...fudge factor - adjust to prevent jerkiness in player2
    }
    //...player2 was changed, so sync player1
    else if((currentTime[rm_followingPlayer] < previousTime[rm_followingPlayer]) || (currentTime[rm_followingPlayer] > previousTime[rm_followingPlayer] + .4)) {
      rm_players[rm_leadingPlayer].sendEvent('SEEK', (currentTime[rm_followingPlayer] - 0.2)); //...fudge factor - adjust to prevent jerkiness in player1
    }
    //...players are out of sync - sync player1 to player2
    else if(currentTime[rm_leadingPlayer] < (currentTime[rm_followingPlayer] - 0.3)) {
      rm_players[rm_leadingPlayer].sendEvent('SEEK', (currentTime[rm_followingPlayer] - .1));
    }
    else if(currentTime[rm_leadingPlayer] > (currentTime[rm_followingPlayer] + 0.3)) {
      rm_players[rm_leadingPlayer].sendEvent('SEEK', (currentTime[rm_followingPlayer]));
    }
    previousTime[rm_leadingPlayer] = currentTime[rm_leadingPlayer];
    previousTime[rm_followingPlayer] = currentTime[rm_followingPlayer];
  }
}

function rm_retrieveCurrentSlideIndex(player) {
  var list = Drupal.settings.rm_player[player.id].slides.timecodes;
  var l = list.length;
  var slide = -1;

  for (var i = 0; i < l; i++) {
    if (player.position > list[i]) {
      slide = i;
    }
  }
  return slide;
}

function rm_sendSeekEvent(player_id, slideIndex) {
  var player = rm_players[player_id];
  if (!player) return false;
  var currentItemPos = Drupal.settings.rm_player[player_id].slides.timecodes[slideIndex];
  if ((typeof(currentItemPos) != 'undefined') && rm_check_buffer(player, currentItemPos)) {
    // Check if the movie is buffered up to the required position.
    if (player.getConfig().state == 'IDLE' || player.getConfig().state == 'PAUSED') {
      player.sendEvent('PLAY', 'true');
    }
    player.sendEvent('SEEK', currentItemPos);
    return true;
  }

  return false;
}

function rm_check_buffer(player, position) {
  var per1 = position / player.info.duration;
  var per2 = player.info.loaded / player.info.filesize;
  if ((per2 == 1) || (per1 < 1 && per1 > 0 && per2 > (per1 + (per1 * 0.1)))) {
    return true;
  }
  return false;
}

function rm_stateListener(player) {
  if (player.newstate != previousState[player.id]) {
    switch(player.newstate) {
      case 'PAUSED':
        rm_players[rm_followingPlayer].sendEvent('PLAY', false);
      break;
      case 'PLAYING':
        // Send the following player the send signal
        rm_players[rm_followingPlayer].sendEvent('PLAY', true);
        // Also mute the following player to ensure sounds don't mix
        //rm_players[rm_followingPlayer].sendEvent('MUTE', true);
      break;
      case 'COMPLETED':
        rm_players[rm_followingPlayer].sendEvent('STOP');
      break;
    }

    if (rm_leadingPlayer != 0) {
      previousState[rm_leadingPlayer] = player.newstate;
    }
    if (rm_followingPlayer != 0) {
      previousState[rm_followingPlayer] = player.newstate;
    }
  }
}

(function ($) {

Drupal.behaviors.rm_addSlideListeners = function(context) {
  $('#tab-slides-detail .slidepager .slidepager-next,#tab-slides-detail .slidepager .slidepager-prev', context).click(function() {
    rm_sendSeekEvent('player_1', $(this).parents('.tabslider:first').get(0).tabSlider.currentItem);
  });

  $('#tab-slides-overview .slide').each(function(i) {
    $(this).click(function() {
      if (rm_sendSeekEvent('player_1', i)) {
        $('.slide-active', $(this).parents('.item')).removeClass('slide-active');
        $(this).addClass('slide-active');
      }
    });
  });
};

Drupal.behaviors.rm_addPlayerLister = function (context) {
  $('embed').bind('mousedown', function(context) {
    rm_leadingPlayer = $(this).attr('name');
    var index = $('embed').index($(this));
    var other_index = ($('embed').eq(index +1).attr('name') == undefined) ? index - 1 : index + 1;
    rm_followingPlayer = $('embed').eq(other_index).attr('name');
  });
};

})(jQuery);

function objLength(obj) {
  var count = 0;
  for (var i in obj) {
    count++;
  };
  return count;
};