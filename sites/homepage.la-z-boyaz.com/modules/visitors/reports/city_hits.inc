<?php

/**
 * @file
 * Recent city hits report for the visitors module.
 */

/**
 * Display city hits report.
 *
 * @return
 *   string city hits report html source
 */
function visitors_city_hits($country, $city) {
  $date_format    = variable_get('date_format_short_custom', 'Y-m-d H:i:s');
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('ID'), 'field' => 'visitors_id', 'sort' => 'desc'),
    array('data' => t('Date'), 'field' => 'visitors_date_time'),
    array('data' => t('URL'), 'field' => 'visitors_url'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations'))
  );

  $original_country = ($country == '(none)') ? '' : $country;
  $original_city = ($city == '(none)') ? '' : $city;

  $sql = sprintf('SELECT
    v.visitors_id,
    v.visitors_uid,
    v.visitors_date_time,
    v.visitors_title,
    v.visitors_path,
    v.visitors_url,
    u.name,
    u.uid
    FROM {visitors} v
    LEFT JOIN {users} u
    ON u.uid=v.visitors_uid
    WHERE v.visitors_country_name = \'%%s\'
    AND v.visitors_city = \'%%s\'
    AND %s ' . tablesort_sql($header),
    visitors_date_filter_sql_condition()
  );

  $count_sql = sprintf('SELECT
    COUNT(*) AS count
    FROM {visitors}
    WHERE visitors_country_name = \'%%s\'
    AND visitors_city = \'%%s\'
    AND %s ',
    visitors_date_filter_sql_condition()
  );

  $query = db_query($count_sql);
  $data = db_fetch_object($query);
  $results = pager_query($sql, $items_per_page, 0, $count_sql,
    $original_country, $original_city);

  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);

  $error_404 = TRUE;
  while ( $data = db_fetch_object($results) ) {
    $error_404 = FALSE;
    $user = user_load(array('uid' => $data->visitors_uid));
    $user_page = theme('username', $data);

    $rows[] = array(
      ++$i,
      $data->visitors_id,
      format_date(
        $data->visitors_date_time,
        'custom',
        $date_format
      ),
      check_plain(
        $data->visitors_title) . '<br/>' . l($data->visitors_path,
        $data->visitors_url
      ),
      $user_page,
      l(t('details'), 'visitors/hits/' . $data->visitors_id)
    );
  }

  if ($error_404) {
    return drupal_not_found();
  }

  drupal_set_title(t('Hits from') . ' ' . t($city) . ', ' . t($country));

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager');

  return $output;
}

