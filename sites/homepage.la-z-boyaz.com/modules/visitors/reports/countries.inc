<?php

/**
 * @file
 * Countries report for the visitors module.
 */

/**
 * Display countries report.
 *
 * @return
 *   string countries report html source
 */
function visitors_countries() {
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('Country'), 'field' => 'visitors_country_name'),
    array('data' => t('Count'), 'field' => 'count', 'sort' => 'desc'),
  );

  $sql = sprintf('SELECT
    v.visitors_country_name,
    COUNT(v.visitors_country_name) AS count
    FROM {visitors} v
    WHERE %s
    GROUP BY v.visitors_country_name ' . tablesort_sql($header),
    visitors_date_filter_sql_condition()
  );

  $count_sql = sprintf('SELECT
    COUNT(DISTINCT visitors_country_name) AS count
    FROM {visitors}
    WHERE %s',
    visitors_date_filter_sql_condition()
  );

  $query = db_query($count_sql);
  $data = db_fetch_object($query);
  $results = pager_query($sql, $items_per_page, 0, $count_sql);
  $rows = array();
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);

  while ($data = db_fetch_object($results)) {
    if ($data->visitors_country_name == '') {
        $data->visitors_country_name = '(none)';
    }
    $rows[] = array(
      ++$i,
      l(
        t($data->visitors_country_name),
        'visitors/countries/' . $data->visitors_country_name
      ),
      $data->count
    );
  }

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager');

  return $output;
}

