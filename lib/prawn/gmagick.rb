require "prawn"
require_relative "../image"

class Gmagick < Prawn::Images::Image
  attr_accessor :width, :height, :bits
  attr_accessor :scaled_width, :scaled_height
  attr_accessor :gimage

  def self.can_render? image_blob
    GMagick::Image.format(image_blob) ? true : false
  end

  def initialize image_blob
    self.gimage = GMagick::Image.new image_blob
    self.bits = gimage.depth
    self.width = gimage.width
    self.height = gimage.height
  end

  def build_pdf_object(document)
    obj = document.ref!(
      Type: :XObject,
      Subtype: :Image,
      ColorSpace: gimage.colorspace,
      Height: height,
      Width: width,
      BitsPerComponent: bits
    )

    obj << gimage.unpack
    obj.stream.filters << { FlateDecode: nil }

    alpha_mask = self.gimage.alpha_unpack
    if alpha_mask.unpack("C*").uniq.length > 1
      smask_obj = document.ref!(
              :Type             => :XObject,
              :Subtype          => :Image,
              :Height           => height,
              :Width            => width,
              :BitsPerComponent => bits,
              :ColorSpace       => :DeviceGray,
              :Decode           => [0, 1]
      )
      smask_obj.stream << alpha_mask
      obj.data[:SMask] = smask_obj
    end

    obj
  end
end

Prawn.image_handler.unregister Prawn::Images::PNG
Prawn.image_handler.register Gmagick
