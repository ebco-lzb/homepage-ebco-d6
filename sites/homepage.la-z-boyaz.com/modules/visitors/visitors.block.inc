<?php

/* vim: set filetype=php : */

/**
 * @file
 * Current visitor block.
 */

/**
 * Implementation of hook_block().
 */
function visitors_block($op = 'list', $delta = 0, $edit = array()) {
  if ($op == 'list') {
    $blocks[0]['info'] = t('Current visitor');
    return $blocks;
  }

  if ($op == 'view') {
    $block['subject'] = t('Visitors');
    $block['content'] = theme('visitors_block_view');
    return $block;
  }
}

/**
 * Implement a theme function for block output.
 */
function theme_visitors_block_view() {
  $list_items = array();

  visitors_block_show_total_visitors($list_items);
  visitors_block_show_unique_visitors($list_items);
  visitors_block_registered_user($list_items);
  visitors_block_last_registered_user($list_items);
  visitors_block_published_nodes($list_items);
  visitors_block_user_ip($list_items);
  visitors_block_since_date($list_items);

  return theme('item_list', $list_items);
}

/**
 * Display total visitors count to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_show_total_visitors(&$list_items) {
  if (variable_get('visitors_show_total_visitors', 1)) {
    $query = 'SELECT COUNT(*) FROM {visitors}';
    $count = db_result(db_query($query));
    $list_items[] = t('Total Visitors: %visitors',
      array('%visitors' => $count)
    );
  }
}

/**
 * Display unique visitors count to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_show_unique_visitors(&$list_items) {
  if (variable_get('visitors_show_unique_visitor', 1)) {
    $query = 'SELECT COUNT(DISTINCT visitors_ip) FROM {visitors}';
    $unique_visitors = db_result(db_query($query));
    $list_items[] = t('Unique Visitors: %unique_visitors',
      array('%unique_visitors' => $unique_visitors));
  }
}

/**
 * Display registered users count to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_registered_user(&$list_items) {
  if (variable_get('visitors_registered_user', 1)) {
    $query = 'SELECT COUNT(*) FROM {users} WHERE uid <> 0';
    $registered_user = db_result(db_query($query));
    $list_items[] = t('Registered Users: %registered_user',
      array('%registered_user' => $registered_user));
  }
}

/**
 * Display last registered user to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_last_registered_user(&$list_items) {
  if (variable_get('visitors_last_registered_user', 1)) {
    $query = 'SELECT * FROM {users} ORDER BY uid DESC';
    $last_user = db_fetch_object(db_query_range($query, 0, 1));
    $list_items[] = t('Last Registered User: !last_user',
      array('!last_user' => theme('username', $last_user)));
  }
}

/**
 * Display published nodes count to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_published_nodes(&$list_items) {
  if (variable_get('visitors_published_nodes', 1)) {
    $query = 'SELECT COUNT(*) FROM {node} WHERE status = 1';
    $published_nodes = db_result(db_query($query));
    $list_items[] = t('Published Nodes: %published_nodes',
      array('%published_nodes' => $published_nodes));
  }
}

/**
 * Display user ip to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_user_ip(&$list_items) {
  if (variable_get('visitors_user_ip', 1)) {
    $list_items[] = t('Your IP: %user_ip',
      array('%user_ip' => visitors_get_ip()));
  }
}

/**
 * Display the start date statistics to visitors block.
 *
 * @param $list_items
 *   array associative array of variables, use for theme function
 */
function visitors_block_since_date(&$list_items) {
  if (variable_get('visitors_since_date', 1)) {
    $query = 'SELECT visitors_date_time
      FROM {visitors}
      ORDER BY visitors_date_time ASC';
    $since_date = db_result(db_query_range($query, 0, 1));
    $list_items[] = t('Since: %since_date',
      array('%since_date' => format_date($since_date, 'short')));
  }
}

