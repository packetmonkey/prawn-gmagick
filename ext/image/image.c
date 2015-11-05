#include <ruby.h>
#include <string.h>
#include <wand/magick_wand.h>

VALUE format(VALUE self, VALUE rb_image_blob)
{
  const char *format = NULL;
  long double image_blob_length = RSTRING_LEN(rb_image_blob);
  char *image_blob = RSTRING_PTR(rb_image_blob);

  MagickCoreGenesis(".", MagickFalse);

  MagickWand *magick_wand = NewMagickWand();
  MagickReadImageBlob(magick_wand, (const unsigned char *)image_blob, image_blob_length);

  format = MagickGetImageFormat(magick_wand);

  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();

  if (format) {
    return rb_str_new2(format);
  } else {
    return Qnil;
  }
}

VALUE image_initialize(VALUE self, VALUE rb_image_blob)
{
  rb_funcall(self, rb_intern("blob="), 1, rb_image_blob);
  return Qtrue;
}

VALUE image_depth(VALUE self)
{
  MagickCoreGenesis(".", MagickFalse);
  MagickWand *magick_wand = NewMagickWand();

  VALUE rb_blob = rb_funcall(self, rb_intern("blob"), 0);
  long double image_blob_length = RSTRING_LEN(rb_blob);
  char *image_blob = RSTRING_PTR(rb_blob);

  MagickReadImageBlob(magick_wand, image_blob, image_blob_length);

  unsigned long depth = MagickGetImageDepth(magick_wand);

  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();

  return INT2NUM(depth);
}

VALUE image_width(VALUE self)
{
  MagickCoreGenesis(".", MagickFalse);
  MagickWand *magick_wand = NewMagickWand();

  VALUE rb_blob = rb_funcall(self, rb_intern("blob"), 0);
  long double image_blob_length = RSTRING_LEN(rb_blob);
  char *image_blob = RSTRING_PTR(rb_blob);

  MagickReadImageBlob(magick_wand, image_blob, image_blob_length);

  unsigned long width = MagickGetImageWidth(magick_wand);

  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();


  return INT2NUM(width);
}

VALUE image_height(VALUE self)
{
  MagickCoreGenesis(".", MagickFalse);
  MagickWand *magick_wand = NewMagickWand();

  VALUE rb_blob = rb_funcall(self, rb_intern("blob"), 0);
  long double image_blob_length = RSTRING_LEN(rb_blob);
  char *image_blob = RSTRING_PTR(rb_blob);

  MagickReadImageBlob(magick_wand, image_blob, image_blob_length);

  unsigned long height = MagickGetImageHeight(magick_wand);

  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();

  return INT2NUM(height);
}

VALUE image_colorspace(VALUE self)
{
  MagickCoreGenesis(".", MagickFalse);
  MagickWand *magick_wand = NewMagickWand();

  VALUE rb_blob = rb_funcall(self, rb_intern("blob"), 0);
  long double image_blob_length = RSTRING_LEN(rb_blob);
  char *image_blob = RSTRING_PTR(rb_blob);

  MagickReadImageBlob(magick_wand, image_blob, image_blob_length);

  ColorspaceType color_space = MagickGetImageColorspace(magick_wand);

  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();

  if (color_space == RGBColorspace) {
    return ID2SYM(rb_intern("DeviceRGB"));
  } else if (color_space == sRGBColorspace) {
    return ID2SYM(rb_intern("DeviceRGB"));
  } else if (color_space == CMYKColorspace) {
    return ID2SYM(rb_intern("DeviceCMYK"));
  } else {
    rb_raise(rb_eTypeError, "Unknown colorspace");
  }
}

VALUE image_unpack(VALUE self)
{
  MagickCoreGenesis(".", MagickFalse);
  MagickWand *magick_wand = NewMagickWand();

  VALUE rb_blob = rb_funcall(self, rb_intern("blob"), 0);
  long double image_blob_length = RSTRING_LEN(rb_blob);
  char *image_blob = RSTRING_PTR(rb_blob);

  MagickReadImageBlob(magick_wand, image_blob, image_blob_length);

  ColorspaceType color_space = MagickGetImageColorspace(magick_wand);
  unsigned long height = MagickGetImageHeight(magick_wand);
  unsigned long width = MagickGetImageWidth(magick_wand);
  int pixel_count = height * width;
  int buffer_size;
  char color_format[8];
  unsigned char *pixels;

  if (color_space == RGBColorspace) {
    buffer_size = 3 * pixel_count;
    strcpy(color_format, "RGB");
  } else if (color_space == sRGBColorspace) {
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

  VALUE rb_pixels = rb_str_new((const char *)pixels, buffer_size);

  free(pixels);
  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();

  return rb_pixels;
}

VALUE image_alpha_unpack(VALUE self)
{
  MagickCoreGenesis(".", MagickFalse);
  MagickWand *magick_wand = NewMagickWand();

  VALUE rb_blob = rb_funcall(self, rb_intern("blob"), 0);
  long double image_blob_length = RSTRING_LEN(rb_blob);
  char *image_blob = RSTRING_PTR(rb_blob);

  MagickReadImageBlob(magick_wand, image_blob, image_blob_length);

  unsigned long height = MagickGetImageHeight(magick_wand);
  unsigned long width = MagickGetImageWidth(magick_wand);
  int pixel_count = height * width;
  unsigned char *pixels;

  pixels = (unsigned char *)malloc(pixel_count * sizeof(char));

  MagickGetImagePixels(magick_wand, 0, 0, width, height, "A", CharPixel, pixels);

  VALUE rb_pixels = rb_str_new((const char *)pixels, pixel_count);

  free(pixels);
  DestroyMagickWand(magick_wand);
  MagickCoreTerminus();

  return rb_pixels;
}

void Init_image(void)
{
  // Define our name spaces and classes
  VALUE m_gmagick = rb_define_module("GMagick");
  VALUE c_image   = rb_define_class_under(m_gmagick, "Image", rb_cObject);

  // Define our class methods
  rb_define_singleton_method(c_image, "format", format, 1);

  // Define our instance methods
  rb_define_attr(c_image, "blob", 1, 1);

  rb_define_method(c_image, "initialize", image_initialize, 1);
  rb_define_method(c_image, "depth", image_depth, 0);
  rb_define_method(c_image, "width", image_width, 0);
  rb_define_method(c_image, "height", image_height, 0);
  rb_define_method(c_image, "colorspace", image_colorspace, 0);
  rb_define_method(c_image, "unpack", image_unpack, 0);
  rb_define_method(c_image, "alpha_unpack", image_alpha_unpack, 0);
}
