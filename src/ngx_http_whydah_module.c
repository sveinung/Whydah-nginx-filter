#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char* ngx_http_whydah(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static char* ngx_http_whydah_roles(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static void* ngx_http_whydah_create_loc_conf(ngx_conf_t* cf);
static char* ngx_http_whydah_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);

typedef struct {
    ngx_flag_t enable;
} ngx_http_whydah_loc_conf_t;

static ngx_command_t  ngx_http_whydah_commands[] = {
    { ngx_string("whydah"),
      NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
      ngx_http_whydah,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

    { ngx_string("whydah_roles"),
      NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
      ngx_http_whydah_roles,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};

static ngx_http_module_t ngx_http_whydah_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    ngx_http_whydah_create_loc_conf,  /* create location configuration */
    ngx_http_whydah_merge_loc_conf /* merge location configuration */
};

ngx_module_t ngx_http_whydah_module = {
    NGX_MODULE_V1,
    &ngx_http_whydah_module_ctx, /* module context */
    ngx_http_whydah_commands,   /* module directives */
    NGX_HTTP_MODULE,                 /* module type */
    NULL,                            /* init master */
    NULL,                            /* init module */
    NULL,                            /* init process */
    NULL,                            /* init thread */
    NULL,                            /* exit thread */
    NULL,                            /* exit process */
    NULL,                            /* exit master */
    NGX_MODULE_V1_PADDING
};

static char* ngx_http_whydah(ngx_conf_t* cf, ngx_command_t* cmd, void* conf)
{
//    ngx_http_whydah_loc_conf_t* passthrough_loc_conf = conf;

    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "configuring whydah");

    return NGX_CONF_OK;
}

static char* ngx_http_whydah_roles(ngx_conf_t* cf, ngx_command_t* cmd, void* conf)
{
//    ngx_http_whydah_loc_conf_t* passthrough_loc_conf = conf;
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "configuring whydah_roles");

    ngx_uint_t i;
    ngx_str_t* value = cf->args->elts;

    for (i = 1; i < cf->args->nelts; i++)
    {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "role \"%V\"", &value[i]);
    }

    return NGX_CONF_OK;
}

static void* ngx_http_whydah_create_loc_conf(ngx_conf_t* cf)
{
    ngx_http_whydah_loc_conf_t* conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_whydah_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    return conf;
}

static char* ngx_http_whydah_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child)
{
    return NGX_CONF_OK;
}
