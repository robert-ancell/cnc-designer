project('cnc-designer', 'c',
        version : '0.1',
        default_options : ['warning_level=3', 'c_std=c11'])

gnome = import('gnome')

gtk_dep = dependency('gtk+-3.0')
json_glib_dep = dependency('json-glib-1.0')

install_data('cnc-designer.desktop', install_dir: join_paths(get_option('datadir'), 'applications'))

subdir('src')
