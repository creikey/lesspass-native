#include <gdnative_api_struct.gen.h>

#define LPCLI_ERROR(what) api->godot_print_error(what, __func__, __FILE__, __LINE__)

#define LP_STATIC
#define LP_IMPLEMENTATION
#define PBKDF2_SHA256_STATIC
#include "lp.h"

#include "lpcli.h"

#include <string.h>

// godot callbacks are in structs and modular
const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

// library constructor
void *LessPass_constructor(godot_object *p_instance, void *p_method_data);

// library destructor
void LessPass_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);

// exported function
godot_variant LessPass_get_data(godot_object *p_instance, void *p_method_data,
                                void *p_user_data, int p_num_args, godot_variant **p_args);

godot_variant LessPass_get_error(godot_object *p_instance, void *p_method_data,
                                 void *p_user_data, int p_num_args, godot_variant **p_args);

// called on library load by godot
void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options)
{
    api = p_options->api_struct;

    for (int i = 0; i < api->num_extensions; i++)
    {
        switch (api->extensions[i]->type)
        {
        case GDNATIVE_EXT_NATIVESCRIPT:
        {
            nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i]; // nativescript callbacks come from godot
        };
        break;
        default:
            break;
        }
    }
}

// library unloading from godot
void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options)
{
    api = NULL;
    nativescript_api = NULL;
}

// nativescript module loading
void GDN_EXPORT godot_nativescript_init(void *p_handle)
{
    // creating new GDScript node in C
    godot_instance_create_func create = {NULL, NULL, NULL};
    create.create_func = &LessPass_constructor;
    godot_instance_destroy_func destroy = {NULL, NULL, NULL};
    destroy.destroy_func = &LessPass_destructor;
    nativescript_api->godot_nativescript_register_class(p_handle, "LessPass", "Reference",
                                                        create, destroy);

    // register method onto new node
    godot_instance_method get_data = {NULL, NULL, NULL};
    get_data.method = &LessPass_get_data;
    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};
    nativescript_api->godot_nativescript_register_method(p_handle, "LessPass", "get_data",
                                                         attributes, get_data);
    godot_instance_method get_error = {NULL, NULL, NULL};
    get_error.method = &LessPass_get_error;
    attributes.rpc_type = GODOT_METHOD_RPC_MODE_DISABLED;
    nativescript_api->godot_nativescript_register_method(p_handle, "LessPass", "get_error", attributes, get_error);
}

typedef enum
{
    GODOT_LPCLI_OK,
    GODOT_LPCLI_ZERO_LENGTH
} GODOT_LPCLI_ERROR;

const char *lcpli_error_strings[] = {
    "ok",
    "password of zero length returned"};

const char *godot_lpcli_strerror(int godot_lpcli_errno)
{
    return lcpli_error_strings[godot_lpcli_errno];
}

// properties
typedef struct user_data_struct
{
    char data[256];
    GODOT_LPCLI_ERROR err;
} user_data_struct;

void *LessPass_constructor(godot_object *p_instance, void *p_method_data)
{
    user_data_struct *user_data = api->godot_alloc(sizeof(user_data_struct));
    strcpy(user_data->data, "World from GDNative!");
    user_data->err = GODOT_LPCLI_OK;
    return user_data;
}

void LessPass_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    api->godot_free(p_user_data);
}

godot_variant LessPass_get_error(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args)
{
    godot_variant ret;
    user_data_struct *user_data = (user_data_struct *)p_user_data;

    api->godot_variant_new_int(&ret, user_data->err);

    return ret;
}

godot_variant LessPass_get_data(godot_object *p_instance, void *p_method_data,
                                void *p_user_data, int p_num_args, godot_variant **p_args)
{
    godot_variant error_return;
    api->godot_variant_new_int(&error_return, 1);

    if (p_num_args != 3)
    {
        LPCLI_ERROR("Incorrect number of arguments. Arguments are [site],[login],[password]");
        return error_return;
    }

    // everything must use variants
    godot_string data;
    godot_variant ret;
    user_data_struct *user_data = (user_data_struct *)p_user_data;

    LP_CTX ctx;
    LP_CTX_init(&ctx);

    godot_string site_name = api->godot_variant_as_string(p_args[0]);
    godot_string user_name = api->godot_variant_as_string(p_args[1]);
    godot_string password = api->godot_variant_as_string(p_args[2]); // TODO make more secure somehow

    godot_char_string site_name_cs = api->godot_string_utf8(&site_name);
    godot_char_string user_name_cs = api->godot_string_utf8(&user_name);
    godot_char_string password_cs = api->godot_string_utf8(&password);

    const char *site_name_c = api->godot_char_string_get_data(&site_name_cs);
    const char *user_name_c = api->godot_char_string_get_data(&user_name_cs);
    const char *password_c = api->godot_char_string_get_data(&password_cs);

    int gen_ret = LP_generate(&ctx, site_name_c, user_name_c, password_c);
    if (gen_ret < 1)
    {
        LPCLI_ERROR("Zero length output password");
        user_data->err = GODOT_LPCLI_ZERO_LENGTH;
        return error_return;
    }
    else
    {
        user_data->err = GODOT_LPCLI_OK;
    }

    api->godot_string_new(&data);
    api->godot_string_parse_utf8(&data, ctx.buffer);
    api->godot_variant_new_string(&ret, &data);
    api->godot_string_destroy(&data);
    api->godot_string_destroy(&site_name);
    api->godot_string_destroy(&user_name);
    api->godot_string_destroy(&password);

    return ret;
}