#include <ruby.h>
#include <string.h>
#include <wand/magick_wand.h>

VALUE format(VALUE self, VALUE rb_image_blob)
{
  const char *format = NULL;
  long double image_blob_length = RSTRING_LEN(rb_image_blob);
  char *image_blob = RSTRING_PTR(rb_image_blob);

  InitializeMagick(".");
  MagickWand *magick_wand = NewMagickWand();
  MagickReadImageBlob(magick_wand, (const unsigned char *)image_blob, image_blob_length);
  format = MagickGetImageFormat(magick_wand);

  if (format) {
    return rb_str_new2(format);
  } else {
    return Qnil;
  }
}

VALUE image_initialize(VALUE self, VALUE rb_image_blob)
{
  MagickWand *magick_wand;

  InitializeMagick(".");

  magick_wand = NewMagickWand();

  long double image_blob_length = RSTRING_LEN(rb_image_blob);
  char *image_blob = RSTRING_PTR(rb_image_blob);
  MagickReadImageBlob(magick_wand, (const unsigned char *)image_blob, image_blob_length);

  VALUE rb_wand = Data_Wrap_Struct(rb_cObject, NULL, NULL, magick_wand);

  rb_funcall(self, rb_intern("wand="), 1, rb_wand);
  rb_funcall(self, rb_intern("blob="), 1, rb_image_blob);

  return Qtrue;
}

VALUE image_depth(VALUE self)
{
  VALUE rb_wand = rb_funcall(self, rb_intern("wand"), 0);
  MagickWand *magick_wand;
  Data_Get_Struct(rb_wand, MagickWand, magick_wand);

  unsigned long depth = MagickGetImageChannelDepth(magick_wand, RedChannel);
  return INT2NUM(depth);
}

VALUE image_width(VALUE self)
{
  VALUE rb_wand = rb_funcall(self, rb_intern("wand"), 0);
  MagickWand *magick_wand;
  Data_Get_Struct(rb_wand, MagickWand, magick_wand);
  unsigned long width = MagickGetImageWidth(magick_wand);
  return INT2NUM(width);
}

VALUE image_height(VALUE self)
{
  VALUE rb_wand = rb_funcall(self, rb_intern("wand"), 0);
  MagickWand *magick_wand;
  Data_Get_Struct(rb_wand, MagickWand, magick_wand);
  unsigned long height = MagickGetImageHeight(magick_wand);
  return INT2NUM(height);
}

VALUE image_colorspace(VALUE self)
{
  VALUE rb_wand = rb_funcall(self, rb_intern("wand"), 0);
  MagickWand *magick_wand;
  Data_Get_Struct(rb_wand, MagickWand, magick_wand);
  ColorspaceType color_space = MagickGetImageColorspace(magick_wand);
  if (color_space == RGBColorspace) {
    return ID2SYM(rb_intern("DeviceRGB"));
  } else if (color_space == CMYKColorspace) {
    return ID2SYM(rb_intern("DeviceCMYK"));
  } else {
    rb_raise(rb_eTypeError, "Unknown colorspace");
  }
}

VALUE image_unpack(VALUE self)
{
  VALUE rb_wand = rb_funcall(self, rb_intern("wand"), 0);
  MagickWand *magick_wand;
  Data_Get_Struct(rb_wand, MagickWand, magick_wand);
  ColorspaceType color_space = MagickGetImageColorspace(magick_wand);

  int height = NUM2INT( rb_funcall(self, rb_intern("height"), 0) );
  int width = NUM2INT( rb_funcall(self, rb_intern("width"), 0) );
  int pixel_count = height * width;
  int buffer_size;
  char color_format[8];
  unsigned char *pixels;

  if (color_space == RGBColorspace) {
     buffer_size = 3 * pixel_count;
     strcpy(color_format, "RGB");
  } else if (color_space == CMYKColorspace) {
     buffer_size = 4 * pixel_count;
     strcpy(color_format, "CMYK");
  } else {
    rb_raise(rb_eTypeError, "Unknown colorspace");
  }

  pixels = (unsigned char *)malloc(buffer_size * sizeof(char));
  MagickGetImagePixels(magick_wand, 0, 0, width, height, color_format, CharPixel, pixels);

  return rb_str_new((const char *)pixels, buffer_size);
}

VALUE image_alpha_unpack(VALUE self)
{
  VALUE rb_wand = rb_funcall(self, rb_intern("wand"), 0);
  MagickWand *magick_wand;
  Data_Get_Struct(rb_wand, MagickWand, magick_wand);
  int height = NUM2INT( rb_funcall(self, rb_intern("height"), 0) );
  int width = NUM2INT( rb_funcall(self, rb_intern("width"), 0) );
  int pixel_count = height * width;
  unsigned char *pixels;

  pixels = (unsigned char *)malloc(pixel_count * sizeof(char));
  MagickGetImagePixels(magick_wand, 0, 0, width, height, "A", CharPixel, pixels);
  return rb_str_new((const char *)pixels, pixel_count);
}

void Init_gmagick(void)
{
  // Deine our name spaces and classes
  VALUE m_gmagick = rb_define_module("GMagick");
  VALUE c_image = rb_define_class_under(m_gmagick, "Image", rb_cObject);

  // Define our class methods
  rb_define_singleton_method(c_image, "format", format, 1);

  // Define our instance methods
  rb_define_attr(c_image, "blob", 1, 1);
  rb_define_attr(c_image, "wand", 1, 1);
  rb_define_method(c_image, "initialize", image_initialize, 1);
  rb_define_method(c_image, "depth", image_depth, 0);
  rb_define_method(c_image, "width", image_width, 0);
  rb_define_method(c_image, "height", image_height, 0);
  rb_define_method(c_image, "colorspace", image_colorspace, 0);
  rb_define_method(c_image, "unpack", image_unpack, 0);
  rb_define_method(c_image, "alpha_unpack", image_alpha_unpack, 0);
}
