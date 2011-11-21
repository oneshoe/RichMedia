<?php
// $Id$

/**
 * @file
 * Template file for PHPtemplate engine containing pager overrides.
 */

/**
 * Override for theme_pager() theme function.
 */
function richmedia_pager($tags = array(), $limit = 10, $element = 0, $parameters = array(), $quantity = 9) {
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', (isset($tags[0]) ? $tags[0] : t('« first')), $limit, $element, $parameters);
  $li_previous = theme('pager_previous', (isset($tags[1]) ? $tags[1] : t('‹ previous')), $limit, $element, 1, $parameters);
  $li_next = theme('pager_next', (isset($tags[3]) ? $tags[3] : t('next ›')), $limit, $element, 1, $parameters);
  $li_last = theme('pager_last', (isset($tags[4]) ? $tags[4] : t('last »')), $limit, $element, $parameters);

  $output_left = '';
  $output_right = '';

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $output_left .= '<div class="pager-first">'. $li_first .'</div>';
    }
    if ($li_previous) {
      $output_left .= '<div class="pager-previous">'. $li_previous .'</div>';
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      $first = FALSE;
      if ($i > 1) {
        $items[] = array(
          'class' => 'pager-ellipsis first-pager',
          'data' => '…',
        );
        $first = TRUE;
      }
      // Now generate the actual pager piece.


      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $class = 'pager-item';
          if (!$first) {
            $class .= ' first-pager';
            $first = $i;
          }
          $items[] = array(
            'class' => $class,
            'data' => theme('pager_previous', $i, $limit, $element, ($pager_current - $i), $parameters),
          );
        }
        if ($i == $pager_current) {
          $class = 'pager-current';
          if (!$first) {
            $class .= ' first-pager';
            $first = $i;
          }
          if ($i == $pager_last) $class .= ' last-pager';
          $items[] = array(
            'class' => $class,
            'data' => '<span>'. $i .'</span>',
          );
        }
        if ($i > $pager_current) {
          $class = 'pager-item';
          if ($i == $pager_max) $class .= ' last-pager';
          $items[] = array(
            'class' => $class,
            'data' => theme('pager_next', $i, $limit, $element, ($i - $pager_current), $parameters),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => 'pager-ellipsis last-pager',
          'data' => '…',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $output_right .= '<div class="pager-next">'. $li_next .'</div>';
    }
    if ($li_last) {
      $output_right .= '<div class="pager-last">'. $li_last .'</div>';
    }

    // Generate the bundled left controls.
    $output = '';
    if (count($items)) {
      $output .= '<div class="pager">';

      if ($output_left) {
        $output .= '<div class="pager-left">'. $output_left .'</div>';
      }
      // Add the numeric pager items.
      $output .= theme('item_list', $items);

      // Add the bundled right controls.
      if ($output_right) {
        $output .= '<div class="pager-right">'. $output_right .'</div>';
      }
      $output .= '</div>';
    }

    return $output;
  }
}