resources_src = gnome.compile_resources(
  'cnc-resources',
  'cnc-designer.gresource.xml',
  source_dir : '.',
  c_name : 'cnc'
)

exe = executable('cnc-designer',
                 resources_src,
                 sources : [
                   'cnc-application.c',
                   'cnc-application-window.c',
                   'cnc-designer.c',
                   'cnc-etcher.c',
                   'cnc-gcode-generator.c',
                   'cnc-grbl-client.c',
                   'cnc-layer.c',
                   'cnc-plan.c',
                   'cnc-plan-view.c',
                   'cnc-shape.c',
                 ],
                 dependencies : [ gtk_dep, json_glib_dep ],
                 install : true)

test('basic', exe)
