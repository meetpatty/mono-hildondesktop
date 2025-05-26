#ifndef __HD_PLUGIN_LOADER_MONO_H__
#define __HD_PLUGIN_LOADER_MONO_H__

#include <hildon-desktop/hd-plugin-loader.h>

G_BEGIN_DECLS

typedef struct _HDPluginLoaderMono HDPluginLoaderMono;
typedef struct _HDPluginLoaderMonoClass HDPluginLoaderMonoClass;
typedef struct _HDPluginLoaderMonoPrivate HDPluginLoaderMonoPrivate;

#define HD_TYPE_PLUGIN_LOADER_MONO            (hd_plugin_loader_mono_get_type ())
#define HD_PLUGIN_LOADER_MONO(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), HD_TYPE_PLUGIN_LOADER_MONO, HDPluginLoaderMono))
#define HD_IS_PLUGIN_LOADER_MONO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), HD_TYPE_PLUGIN_LOADER_MONO))
#define HD_PLUGIN_LOADER_MONO_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), HD_TYPE_PLUGIN_LOADER_MONO_CLASS, HDPluginLoaderMonoClass))
#define HD_IS_PLUGIN_LOADER_MONO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), HD_TYPE_PLUGIN_LOADER_MONO_CLASS))
#define HD_PLUGIN_LOADER_MONO_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), HD_TYPE_PLUGIN_LOADER_MONO, HDPluginLoaderMonoClass))

struct _HDPluginLoaderMono
{
  HDPluginLoader parent;

  HDPluginLoaderMonoPrivate *priv;
};

struct _HDPluginLoaderMonoClass
{
  HDPluginLoaderClass parent_class;
};

GType  hd_plugin_loader_mono_get_type  (void);

G_END_DECLS

#endif
