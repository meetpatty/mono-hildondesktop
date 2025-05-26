#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

#include <libhildondesktop/hildon-desktop-plugin.h>

#include "hd-plugin-loader-mono.h"
#include <hildon-desktop/hd-config.h>

#define HD_PLUGIN_LOADER_MONO_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((obj), HD_TYPE_PLUGIN_LOADER_MONO, HDPluginLoaderMonoPrivate))

G_DEFINE_TYPE (HDPluginLoaderMono, hd_plugin_loader_mono, HD_TYPE_PLUGIN_LOADER);

#define CS_PLUGIN_FACTORY_CLASS "HdPluginFactory"
#define CS_PLUGIN_FACTORY_METHOD "GetObjects"
#define CS_PLUGIN_FACTORY_METHOD_FQN CS_PLUGIN_FACTORY_CLASS "." CS_PLUGIN_FACTORY_METHOD

static MonoDomain *domain;
MonoAssembly *assembly;

struct _HDPluginLoaderMonoPrivate 
{
  gboolean initialised;
};

static GtkWidget *
hd_plugin_loader_mono_plugin_settings(GtkWidget *item, GtkWidget *top_level, MonoObject *object)
{
  MonoClass* klass = NULL;
  MonoProperty *prop = NULL;
  
  klass = mono_object_get_class(object);

  if (klass)
  {
    prop = mono_class_get_property_from_name(klass, "SettingsMenuItemHandle");

    if (prop)
    {
      MonoObject* handle_obj = mono_property_get_value(prop, object, NULL, NULL);
      gpointer ptr = *(gpointer*)mono_object_unbox(handle_obj);
      
      if (GTK_IS_WIDGET(ptr))
      {
        return GTK_WIDGET(ptr);
      }
    }
  }

  return NULL;
}

static void 
hd_plugin_loader_mono_destroy_plugin (GtkObject *object, gpointer user_data)
{
  // Can't unload assembly from runtime, do we need separate app domains?
}

static GList * 
hd_plugin_loader_mono_open_module (HDPluginLoaderMono *loader,
                                      GError **error)
{
  HDPluginLoaderMonoPrivate *priv;
  GKeyFile *keyfile;
  GList *objects = NULL;
  GError *keyfile_error = NULL;
  gchar *module_file = NULL;
  gchar *module_name = NULL;
  MonoMethod *method = NULL;

  g_return_val_if_fail (HD_IS_PLUGIN_LOADER_MONO (loader), NULL);

  priv = loader->priv;
 
  keyfile = hd_plugin_loader_get_key_file (HD_PLUGIN_LOADER (loader));
  
  module_file = g_key_file_get_string (keyfile,
                                       HD_PLUGIN_CONFIG_GROUP,
                                       HD_PLUGIN_CONFIG_KEY_PATH,
                                       &keyfile_error);

  if (keyfile_error)
  {
    g_propagate_error (error, keyfile_error);

    return NULL;
  }

  if (g_path_is_absolute (module_file))
  {
    module_name = module_file; 
  }
  else
  {
    module_name = g_build_filename(HD_DESKTOP_MODULE_PATH, module_file, NULL);

    g_free (module_file);
  }

  assembly = mono_domain_assembly_open (domain, module_name);

  if (assembly != NULL)
  {
    MonoImage *image = mono_assembly_get_image(assembly);
    MonoClass *klass = mono_class_from_name(image, "", CS_PLUGIN_FACTORY_CLASS);

    if (klass != NULL)
    {
      method = mono_class_get_method_from_name(klass, CS_PLUGIN_FACTORY_METHOD, 0);
    }
    else
    {
      g_warning ("Failed to find " CS_PLUGIN_FACTORY_CLASS " class in \"%s\"", module_name);
    }    
  }
  else {
    g_warning ("Failed to load \"%s\"", module_name);
  }

  if (method != NULL)
  {
    MonoObject *exc = NULL;
    MonoObject *result = mono_runtime_invoke(method, NULL, NULL, &exc);

    if (exc)
    {
      g_warning ("Exception thrown calling " CS_PLUGIN_FACTORY_METHOD_FQN " in \"%s\"", module_name);
    }
    else
    {
      MonoArray *array = (MonoArray*)result;

      for (int i = 0; i < mono_array_length(array); i++)
      {
        MonoObject *plugin_obj = mono_array_get(array, MonoObject*, i);
        MonoClass* klass = mono_object_get_class(plugin_obj);
        MonoProperty* prop = mono_class_get_property_from_name(klass, "Handle");

        if (prop)
        {
          MonoObject* handle_obj = mono_property_get_value(prop, plugin_obj, NULL, NULL);
          gpointer ptr = *(gpointer*)mono_object_unbox(handle_obj);
          GObject* object = G_OBJECT(ptr);

          g_signal_connect (G_OBJECT (object), 
                              "destroy", 
                              G_CALLBACK (hd_plugin_loader_mono_destroy_plugin), 
                              NULL);

          g_signal_connect (G_OBJECT (object), 
                              "settings", 
                              G_CALLBACK (hd_plugin_loader_mono_plugin_settings), 
                              plugin_obj);

          objects = g_list_append (objects, object);
        }
        else
        {
          g_warning ("Failed to find Handle Property on returned Plugin Object in \"%s\"", module_name);
        }
      }
    }
  }
  else
  {
    g_warning ("Failed to find method " CS_PLUGIN_FACTORY_METHOD_FQN " in  \"%s\"", module_name);
  }

  g_free (module_name);

  return objects;
}

static GList *
hd_plugin_loader_mono_load (HDPluginLoader *loader, GError **error)
{
  GList *objects = NULL;
  GKeyFile *keyfile;
  GError *local_error = NULL;

  g_return_val_if_fail (loader, NULL);
 
  keyfile = hd_plugin_loader_get_key_file (loader);

  if (!keyfile)
  {
    g_set_error (error,
                 hd_plugin_loader_error_quark (),
                 HD_PLUGIN_LOADER_ERROR_KEYFILE,
                 "A keyfile required to load plugins");

    return NULL;
  }

  objects = 
      hd_plugin_loader_mono_open_module (HD_PLUGIN_LOADER_MONO (loader),
                                            &local_error);

  if (local_error) 
  {
    g_propagate_error (error, local_error);

    if (objects)
    {
      g_list_foreach (objects, (GFunc) gtk_widget_destroy, NULL);
      g_list_free (objects);
    }

    return NULL;
  }

  return objects;
}

static void
hd_plugin_loader_mono_finalize (GObject *loader)
{
  HDPluginLoaderMonoPrivate *priv;

  g_return_if_fail (loader != NULL);
  g_return_if_fail (HD_IS_PLUGIN_LOADER_MONO (loader));

  priv = HD_PLUGIN_LOADER_MONO (loader)->priv;

  if (priv->initialised)
  {
    mono_jit_cleanup (domain);
  }

  G_OBJECT_CLASS (hd_plugin_loader_mono_parent_class)->finalize (loader);
}

static void
hd_plugin_loader_mono_init (HDPluginLoaderMono *loader)
{
  loader->priv = HD_PLUGIN_LOADER_MONO_GET_PRIVATE (loader);

  mono_config_parse(NULL);

  domain = mono_jit_init_version ("MonoDesktopPluginDomain", "v2.0.50727");

  loader->priv->initialised = TRUE;
}

static void
hd_plugin_loader_mono_class_init (HDPluginLoaderMonoClass *class)
{
  GObjectClass *object_class;
  HDPluginLoaderClass *loader_class;

  object_class = G_OBJECT_CLASS (class);
  loader_class = HD_PLUGIN_LOADER_CLASS (class);
  
  object_class->finalize = hd_plugin_loader_mono_finalize;

  loader_class->load = hd_plugin_loader_mono_load;

  g_type_class_add_private (object_class, sizeof (HDPluginLoaderMonoPrivate));
}

G_MODULE_EXPORT gchar *
hd_plugin_loader_module_type (void)
{
  return "mono";
}

G_MODULE_EXPORT HDPluginLoader *
hd_plugin_loader_module_get_instance (void)
{
  return g_object_new (HD_TYPE_PLUGIN_LOADER_MONO,NULL);
}
