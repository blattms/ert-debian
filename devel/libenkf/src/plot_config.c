#include <stdlib.h>
#include <util.h>
#include <plot_config.h>
#include <enkf_defaults.h>
#include "config_keys.h"

/** 
    Struct holding basic information used when plotting.
*/

struct plot_config_struct {
  char * plot_path;     /* All the plots will be saved as xxxx files in this directory. */
  char * image_type;    /* Type of plot file - currently only 'png' is tested. */
  char * driver;        /* The driver used by the libplot layer when actually 'rendering' the plots. */
  char * viewer;        /* The executable used when displaying the newly created image - can be NULL - in which case the plots are not displayed in any way. */
  int    errorbar_max;  /* If the number of observations is less than this it is plotted with errorbars - otherwise with lines. */
  int    height;   
  int    width;
  bool   logy;
};


/*****************************************************************/

void plot_config_set_errorbar_max( plot_config_type * plot_config , int errorbar_max ) {
  plot_config->errorbar_max = errorbar_max;
}

void plot_config_set_width(plot_config_type * plot_config , int width) {
  plot_config->width = width;
}

void plot_config_set_height(plot_config_type * plot_config , int height) {
  plot_config->height = height;
}

void plot_config_set_path(plot_config_type * plot_config , const char * plot_path) {
  plot_config->plot_path = util_realloc_string_copy(plot_config->plot_path , plot_path);
  util_make_path( plot_path );
}


void plot_config_set_image_type(plot_config_type * plot_config , const char * image_type) {
  plot_config->image_type = util_realloc_string_copy(plot_config->image_type , image_type);
}


void plot_config_set_viewer(plot_config_type * plot_config , const char * plot_viewer) {
  if (util_file_exists( plot_viewer) && util_is_executable( plot_viewer ))
    plot_config->viewer = util_realloc_string_copy(plot_config->viewer , plot_viewer);
  else {
    plot_config->viewer = util_realloc_string_copy(plot_config->viewer , NULL );
    
    fprintf(stderr , "\n ---------------------------------------------------------------------\n");
    fprintf(stderr , " - The ERT variable \"IMAGE_VIEWER\" has not been set to a valid       -\n");
    fprintf(stderr , " - value; this means that ERT can not display the plots for you -    -\n");
    fprintf(stderr , " - sorry. However the plotfiles are available for later viewing with -\n");
    fprintf(stderr , " - your favorite image viewer software.                              -\n");
    fprintf(stderr , " -                                                                   -\n");
    fprintf(stderr , " - To actually set the IMAGE_VIEWER add the following in your        -\n");
    fprintf(stderr , " - configuration file:                                               -\n");  
    fprintf(stderr , " -                                                                   -\n");
    fprintf(stderr , " - IMAGE_VIEWER    /path/to/binary/which/can/display/graphical/files -\n");
    fprintf(stderr , " ---------------------------------------------------------------------\n\n");
  }
}



void plot_config_set_driver(plot_config_type * plot_config , const char * plot_driver) {
  plot_config->driver = util_realloc_string_copy(plot_config->driver , plot_driver);
}


/*****************************************************************/

void plot_config_set_logy( plot_config_type * plot_config , bool logy) {
  plot_config->logy = logy;
}


void plot_config_toggle_logy( plot_config_type * plot_config ) {
  if (plot_config->logy)
    plot_config->logy = false;
  else
    plot_config->logy = true;
}


bool plot_config_get_logy( const plot_config_type * plot_config ) {
  return plot_config->logy;
}


const char *  plot_config_get_path(const plot_config_type * plot_config ) {
  return plot_config->plot_path;
}


const char *  plot_config_get_image_type(const plot_config_type * plot_config ) {
  return plot_config->image_type;
}

const char *  plot_config_get_viewer(const plot_config_type * plot_config ) {
  return plot_config->viewer;
}  

const char *  plot_config_get_driver(const plot_config_type * plot_config ) {
  return plot_config->driver;
}

int plot_config_get_width(const plot_config_type * plot_config ) {
  return plot_config->width;
}

int plot_config_get_height(const plot_config_type * plot_config ) {
  return plot_config->height;
}


int plot_config_get_errorbar_max( const plot_config_type * plot_config ) {
  return plot_config->errorbar_max;
}



void plot_config_free( plot_config_type * plot_config) {
  free(plot_config->plot_path);
  util_safe_free(plot_config->viewer);
  free(plot_config->image_type);
  free(plot_config->driver );
  free(plot_config);
}


/**
   The plot_config object is instantiated with the default values from enkf_defaults.h
*/
plot_config_type * plot_config_alloc_default() {
  plot_config_type * info        = util_malloc( sizeof * info , __func__);
  info->plot_path                = NULL;
  info->image_type               = NULL;
  info->viewer                   = NULL;
  info->driver                   = NULL;      
  
  plot_config_set_path(info         , DEFAULT_PLOT_PATH );
  plot_config_set_image_type(info   , DEFAULT_IMAGE_TYPE );
  plot_config_set_viewer(info       , DEFAULT_IMAGE_VIEWER );
  plot_config_set_driver(info       , DEFAULT_PLOT_DRIVER );
  plot_config_set_width(info        , DEFAULT_PLOT_WIDTH );
  plot_config_set_height(info       , DEFAULT_PLOT_HEIGHT );
  plot_config_set_errorbar_max(info , DEFAULT_PLOT_ERRORBAR_MAX);
  plot_config_set_logy( info        , DEFAULT_PLOT_LOGY );
  return info;
}



void plot_config_init(plot_config_type * plot_config , const config_type * config ) {
  if (config_item_set( config , PLOT_PATH_KEY))
    plot_config_set_path( plot_config , config_get_value( config , PLOT_PATH_KEY ));

  if (config_item_set( config , PLOT_DRIVER_KEY))
    plot_config_set_driver( plot_config , config_get_value( config , PLOT_DRIVER_KEY ));
  
  if (config_item_set( config , IMAGE_VIEWER_KEY))
    plot_config_set_viewer( plot_config , config_get_value( config , IMAGE_VIEWER_KEY ));

  if (config_item_set( config , PLOT_DRIVER_KEY))
    plot_config_set_driver( plot_config , config_get_value( config , PLOT_DRIVER_KEY ));

  if (config_item_set( config , PLOT_ERRORBAR_MAX_KEY))
    plot_config_set_errorbar_max( plot_config , config_get_value_as_int( config , PLOT_ERRORBAR_MAX_KEY ));
  
  if (config_item_set( config , PLOT_HEIGHT_KEY))
    plot_config_set_height( plot_config , config_get_value_as_int( config , PLOT_HEIGHT_KEY ));

  if (config_item_set( config , PLOT_WIDTH_KEY))
    plot_config_set_width( plot_config , config_get_value_as_int( config , PLOT_WIDTH_KEY ));
}


void plot_config_add_config_items( config_type * config ) {
  config_add_key_value(config , PLOT_HEIGHT_KEY       , false , CONFIG_INT);
  config_add_key_value(config , PLOT_WIDTH_KEY        , false , CONFIG_INT);
  config_add_key_value(config , PLOT_PATH_KEY         , false , CONFIG_STRING);
  config_add_key_value(config , IMAGE_VIEWER_KEY      , false , CONFIG_FILE);
  config_add_key_value(config , PLOT_ERRORBAR_MAX_KEY , false , CONFIG_INT);

  {
    config_item_type * item;
    
    item = config_add_key_value(config , IMAGE_TYPE_KEY , false , CONFIG_STRING);
    config_item_set_common_selection_set( item , 3 , (const char *[3]) {"png" , "jpg" , "psc"});

    item = config_add_key_value(config , PLOT_DRIVER_KEY , false , CONFIG_STRING);
    config_item_set_common_selection_set( item , 2 , (const char *[2]) {"PLPLOT" , "TEXT"});
  }
}


/**
   It would be natural to set the plot settings in the site config
   file; however that is (mainly for reasons of laziness) not
   possible. The plot settings are therefor either the compiled in
   default settings; or what is set by the user in the user config
   file.
*/

void plot_config_fprintf_config( const plot_config_type * plot_config , FILE * stream ) {
  fprintf( stream , CONFIG_COMMENTLINE_FORMAT );
  fprintf( stream , CONFIG_COMMENT_FORMAT , "Here comes configuration information related to plot configuration.");
  fprintf( stream , CONFIG_COMMENT_FORMAT , "When using the GUI only the PLOT_PATH directive is honored.");
  
  if (!util_string_equal( plot_config->plot_path , DEFAULT_PLOT_PATH)) {
    fprintf(stream , CONFIG_KEY_FORMAT      , PLOT_PATH_KEY );
    fprintf(stream , CONFIG_ENDVALUE_FORMAT , plot_config->plot_path );
  }

  if (!util_string_equal( plot_config->image_type , DEFAULT_IMAGE_TYPE)) {
    fprintf(stream , CONFIG_KEY_FORMAT      , IMAGE_TYPE_KEY );
    fprintf(stream , CONFIG_ENDVALUE_FORMAT , plot_config->image_type );
  }
  
  if (!util_string_equal( plot_config->driver , DEFAULT_PLOT_DRIVER)) {
    fprintf(stream , CONFIG_KEY_FORMAT      , PLOT_DRIVER_KEY );
    fprintf(stream , CONFIG_ENDVALUE_FORMAT , plot_config->driver );
  }

  if (!util_string_equal( plot_config->viewer , DEFAULT_IMAGE_VIEWER)) {
    fprintf(stream , CONFIG_KEY_FORMAT      , IMAGE_VIEWER_KEY );
    fprintf(stream , CONFIG_ENDVALUE_FORMAT , plot_config->viewer );
  }

  if (plot_config->errorbar_max != DEFAULT_PLOT_ERRORBAR_MAX) {
    fprintf(stream , CONFIG_KEY_FORMAT , PLOT_ERRORBAR_MAX_KEY);
    fprintf(stream , CONFIG_INT_FORMAT , plot_config->errorbar_max );
    fprintf(stream , "\n");
  }

  if (plot_config->height != DEFAULT_PLOT_HEIGHT) {
    fprintf(stream , CONFIG_KEY_FORMAT , PLOT_HEIGHT_KEY);
    fprintf(stream , CONFIG_INT_FORMAT , plot_config->height );
    fprintf(stream , "\n");
  }

  if (plot_config->width != DEFAULT_PLOT_HEIGHT) {
    fprintf(stream , CONFIG_KEY_FORMAT , PLOT_WIDTH_KEY);
    fprintf(stream , CONFIG_INT_FORMAT , plot_config->width );
    fprintf(stream , "\n");
  }
  
  fprintf(stream , "\n\n");
}

