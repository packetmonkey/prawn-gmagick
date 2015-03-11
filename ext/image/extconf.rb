require "mkmf"

pkg_config("GraphicsMagick")
pkg_config("GraphicsMagick++")
pkg_config("GraphicsMagickWand")
create_makefile("image")
