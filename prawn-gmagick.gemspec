Gem::Specification.new do |s|
  s.name = "prawn-gmagick"
  s.version = "0.0.1"
  s.platform    = Gem::Platform::RUBY
  s.authors = ["Evan Sharp"]
  s.summary = "Use GraphicsMagick to load images in a Prawn docuemnt"
  s.extensions = %w[ext/gmagick/extconf.rb]
  s.require_path = "lib"
  s.add_dependency "prawn", "0.13.0"
  s.add_dependency "rake-compiler"
  s.add_development_dependency "minitest"
end
