#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static char* ngx_http_whydah_roles(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);
static void* ngx_http_whydah_create_loc_conf(ngx_conf_t* cf);
static char* ngx_http_whydah_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);
static ngx_int_t ngx_http_whydah_filter_init(ngx_conf_t* cf);
static ngx_int_t ngx_http_whydah_header_filter(ngx_http_request_t* r);
static ngx_int_t get_user_token_index_from_cookie(ngx_http_request_t* r);
static ngx_int_t redirect_to_login(ngx_http_request_t* r, ngx_str_t login_page_url);

static ngx_str_t user_token_cookie_name = ngx_string("whydahusertoken_sso");

typedef struct {
    ngx_str_t value;
} whydah_role;

typedef struct {
    ngx_flag_t enable;
    ngx_str_t sso_login_webapp_url;
    ngx_array_t* roles;  // whydah_role-s
} ngx_http_whydah_loc_conf_t;

static ngx_command_t  ngx_http_whydah_commands[] = {
    { ngx_string("whydah"),
      NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_whydah_loc_conf_t, enable),
      NULL },

    { ngx_string("whydah_sso_login_webapp_url"),
      NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_whydah_loc_conf_t, sso_login_webapp_url),
      NULL },

    { ngx_string("whydah_roles"),
      NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
      ngx_http_whydah_roles,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};

static ngx_http_module_t ngx_http_whydah_module_ctx = {
    NULL,                          /* preconfiguration */
    ngx_http_whydah_filter_init,   /* postconfiguration */

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

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;

static char* ngx_http_whydah_roles(ngx_conf_t* cf, ngx_command_t* cmd, void* conf)
{
    ngx_http_whydah_loc_conf_t* whydah_conf = conf;
    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "configuring whydah_roles");

    ngx_uint_t i;
    ngx_str_t* value = cf->args->elts;

    for (i = 1; i < cf->args->nelts; i++)
    {
        whydah_role* new_role = ngx_array_push(whydah_conf->roles);
        if (new_role == NULL) {
            return NGX_CONF_ERROR;
        }
        new_role->value.len = value[i].len;
        new_role->value.data = value[i].data;
    }

    return NGX_CONF_OK;
}

static void* ngx_http_whydah_create_loc_conf(ngx_conf_t* cf)
{
    ngx_http_whydah_loc_conf_t* conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_whydah_loc_conf_t));
    if (conf == NULL)
    {
        return NULL;
    }

    conf->enable = NGX_CONF_UNSET;

    conf->roles = ngx_array_create(cf->pool, 4, sizeof(whydah_role));
    if (conf->roles == NULL)
    {
        return NULL;
    }

    return conf;
}

static char* ngx_http_whydah_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child)
{
    ngx_http_whydah_loc_conf_t* prev = parent;
    ngx_http_whydah_loc_conf_t* conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_whydah_filter_init(ngx_conf_t* cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_whydah_header_filter;

    return NGX_OK;
}

static ngx_int_t ngx_http_whydah_header_filter(ngx_http_request_t* r)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "whydah filtering");

    ngx_http_whydah_loc_conf_t* whydah_conf = ngx_http_get_module_loc_conf(r, ngx_http_whydah_module);

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "whydah filtering: %d", whydah_conf->enable);
    if (!whydah_conf->enable)
    {
        return ngx_http_next_header_filter(r);
    }

    ngx_uint_t i;
    whydah_role* role = whydah_conf->roles->elts;
    for (i = 0; i < whydah_conf->roles->nelts; i++)
    {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "role %s", role[i].value.data);
    }

    ngx_int_t sso_cookie_index = get_user_token_index_from_cookie(r);

    if (sso_cookie_index == NGX_DECLINED)
    {
        if (redirect_to_login(r, whydah_conf->sso_login_webapp_url) != NGX_OK)
        {
            return NGX_ERROR;
        }
    }

    return ngx_http_next_header_filter(r);
}

static ngx_int_t get_user_token_index_from_cookie(ngx_http_request_t* r)
{
    ngx_int_t rc;
    ngx_str_t value;

    rc = ngx_http_parse_multi_header_lines(
        &r->headers_in.cookies,
        &user_token_cookie_name,
        &value);

    if (rc == NGX_DECLINED)
    {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "whydahusertoken_sso not found");
    }
    else
    {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "whydahusertoken_sso %s", value.data);
    }

    return rc;
}

static ngx_int_t redirect_to_login(ngx_http_request_t* r, ngx_str_t login_page_url)
{
    ngx_http_clear_location(r);

    r->headers_out.location = ngx_list_push(&r->headers_out.headers);
    if (r->headers_out.location == NULL) {
        return NGX_ERROR;
    }

    r->headers_out.location->hash = 1;
    ngx_str_set(&r->headers_out.location->key, "Location");

    ngx_str_t full_url;
    full_url.data = ngx_pnalloc(r->pool, 128);
    if (full_url.data == NULL) {
        return NGX_ERROR;
    }
    full_url.len = ngx_snprintf(
        full_url.data,
        128,
        "%s?redirectURI=%s%Z",
        login_page_url.data,
        r->headers_in.host->value.data) - full_url.data;

    r->headers_out.location->value.data = full_url.data;
    r->headers_out.location->value.len = full_url.len;

    r->headers_out.status = NGX_HTTP_TEMPORARY_REDIRECT;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "redirecting to %s", full_url.data);

    return NGX_OK;
}
