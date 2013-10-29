<?php
/**
 * Implementation of hook_drush_command().
 */
function lightcron_drush_command() {
  $items = array();

  // The 'make-me-a-sandwich' command
  $items['run-cron-hooks'] = array(
    'description' => "Run a cron hook for specific module or set of modules",
    'arguments' => array(
      'modules' => 'The modules to check for cron hooks',
    ),
    'examples' => array(
      'drush rch scheduler' => 'Run the scheduler cron hook',
      'drush rch feeds xmlsitemap' => 'Run the cron hooks for feeds and xmlsitemap',
    ),
    'aliases' => array('rch'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function lightcron_drush_help($section) {
  switch ($section) {
    case 'drush:run-cron-hooks':
      return dt("This command will let you run cron hooks for one or more modules.");
  }
}

/**
 * Callback for the run-cron-hooks command
 */
function drush_lightcron_run_cron_hooks() {
  $modules = lightcron_parse_arguments(func_get_args(), FALSE);
  foreach ($modules as $module) {
    if (module_exists($module)) {
      $callback = $module . '_cron';
      if (function_exists($callback)) {
        $callback();
        drush_log(dt('!module (!date): cron hook executed.', array('!module' => $module, '!date' => date('Y-m-d H:i:s O'))), 'ok');
      }
      else {
        drush_log(dt('!module does not have a cron hook.', array('!module' => $module)), 'error');
      }
    }
    else {
      drush_log(dt('!module does not exist i.e. not installed and enabled.', array('!module' => $module)), 'error');
    }
  }
}

/**
 * Sanitize user provided arguments to several pm commands.
 *
 * Return an array of arguments off a space and/or comma separated values.
 *
 * @see drush/commands/pm/pm.drush.inc
 */
function lightcron_parse_arguments($args, $dashes_to_underscores = TRUE) {
  // @see drush/includes/drush.inc for _convert_csv_to_array($args)
  $arguments = _convert_csv_to_array($args);
  foreach ($arguments as $key => $argument) {
    $argument = ($dashes_to_underscores) ? strtr($argument, '-', '_') : $argument;
  }
  return $arguments;
}
