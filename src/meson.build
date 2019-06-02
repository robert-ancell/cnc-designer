resources_src = gnome.compile_resources(
  'cnc-resources',
  'cnc-designer.gresource.xml',
  source_dir : '.',
  c_name : 'cnc'
)

exe = executable('cnc-designer',
                 resources_src,
                 sources : [ 'cnc-application.c',
                   'cnc-application-window.c',
                   'cnc-designer.c' ],
                 dependencies : [ gtk_dep ],
                 install : true)

test('basic', exe)
