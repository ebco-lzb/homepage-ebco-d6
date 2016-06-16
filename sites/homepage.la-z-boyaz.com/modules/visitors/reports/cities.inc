<?php

/**
 * @file
 * Cities report for the visitors module.
 */

/**
 * Display cities report.
 *
 * @return
 *   string cities report html source
 */
function visitors_cities($country) {
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('City'), 'field' => 'visitors_city'),
    array('data' => t('Count'), 'field' => 'count', 'sort' => 'desc'),
  );

  $original_country =  ($country == '(none)') ? '' : $country;

  $sql = sprintf('SELECT
    v.visitors_city,
    COUNT(v.visitors_city) AS count
    FROM {visitors} v
    WHERE v.visitors_country_name = \'%%s\'
    AND %s
    GROUP BY v.visitors_city ' . tablesort_sql($header),
    visitors_date_filter_sql_condition()
  );

  $count_sql = sprintf('SELECT
    COUNT(DISTINCT visitors_city) AS count
    FROM {visitors}
    WHERE visitors_country_name = \'%%s\'
    AND %s ',
    visitors_date_filter_sql_condition()
  );

  $query = db_query($count_sql);
  $data = db_fetch_object($query);
  $results = pager_query($sql, $items_per_page, 0, $count_sql,
    $original_country);

  $rows = array();

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);

  $error_404 = TRUE;
  while ( $data = db_fetch_object($results) ) {
    $error_404 = FALSE;
    if ($data->visitors_city == '') {
      $data->visitors_city = '(none)';
    }
    $rows[] = array(
      ++$i,
      l(
        $data->visitors_city,
        'visitors/countries/' . $country . '/' . $data->visitors_city
      ),
      $data->count
    );
  }

  if ($error_404) {
    return drupal_not_found();
  }

  drupal_set_title(t('Visitors from') . ' ' . t($country));

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager');

  return $output;
}

