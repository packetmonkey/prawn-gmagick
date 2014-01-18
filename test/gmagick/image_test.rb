require_relative "../test_helper"

describe "GMagick::Image::format" do
  describe "with a JPEG image" do
    before do
      fixture_path = File.join __dir__, "..", "fixtures", "rgb.jpg"
      @fixture_blob = File.open(fixture_path).read
    end

    it "returns a string containing 'JPEG'" do
      GMagick::Image.format(@fixture_blob).must_equal "JPEG"
    end
  end

  describe "with an unknown image" do
    before do
      fixture_path = File.join __dir__, "..", "fixtures", "random.bin"
      @fixture_blob = File.open(fixture_path).read
    end

    it "returns nil" do
      GMagick::Image.format(@fixture_blob).must_equal nil
    end
  end
end

describe GMagick::Image do
  before do
    fixture_path = File.join __dir__, "..", "fixtures", "rgb.jpg"
    fixture_blob = File.open(fixture_path).read
    @image = GMagick::Image.new fixture_blob
  end

  describe "#depth" do
    it "returns the image bit depth as an integer" do
      @image.depth.must_equal 8
    end
  end

  describe "#width" do
    it "returns the image width in pixels as an integer" do
      @image.width.must_equal 13
    end
  end

  describe "#height" do
    it "returns the image height in pixels as an integer" do
      @image.height.must_equal 17
    end
  end

  describe "#colorspace" do
    describe "with an RGB image" do
      it "returns the symbol :DeviceRGB" do
        @image.colorspace.must_equal :DeviceRGB
      end
    end

    describe "with a CMYK image" do
      before do
        fixture_path = File.join __dir__, "..", "fixtures", "cmyk.jpg"
        fixture_blob = File.open(fixture_path).read
        @image = GMagick::Image.new fixture_blob
      end

      it "returns the symbol :DeviceCMYK" do
        @image.colorspace.must_equal :DeviceCMYK
      end
    end
  end

  describe "#unpack" do
    describe "with an RGB image" do
      it "returns a string containing the pixel color information" do
        @image.unpack.unpack("CCC").must_equal [255, 38, 0]
      end
    end

    describe "with a CMYK image" do
      before do
        fixture_path = File.join __dir__, "..", "fixtures", "cmyk.jpg"
        fixture_blob = File.open(fixture_path).read
        @image = GMagick::Image.new fixture_blob
      end

      it "returns a string containing the pixel color information" do
        @image.unpack.unpack("CCCC").must_equal [0, 255, 255, 0]
      end
    end
  end

  describe "#unpack_alpha" do
    describe "with an opaque JPEG" do
      it "returns a string containing the pixel alpha information" do
        @image.alpha_unpack.unpack("CCCC").must_equal [255, 255, 255, 255]
      end
    end

    describe "with an transparent GIF" do
      before do
        fixture_path = File.join __dir__, "..", "fixtures", "rgb_alpha.gif"
        fixture_blob = File.open(fixture_path).read
        @image = GMagick::Image.new fixture_blob
      end

      it "returns a string containing the pixel alpha information" do
         @image.alpha_unpack.unpack("C*").slice(11, 3).must_equal [255, 255, 0]
      end
    end
  end
end
