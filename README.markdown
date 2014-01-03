# prawn-gmagick

A [graphicsmagick](http:////www.graphicsmagick.org/) binding to add support for
additional image formats to [prawn](https://github.com/prawnpdf/prawn).

## Usage

**Gemfile**

```ruby
gem "prawn-gmagick"
```

**Your code**

```ruby
require "prawn/gmagick"
```

## Supported Image Formats
prawn-gmagick will try to load any image passed to it that graphicsmagick can
identify. To see what formats your installation supports run `gm version`.

## Found a bug?
Open a [github issue](https://github.com/packetmonkey/prawn-gmagick/issues)

## Contributing & Development
1. Fork the project.
2. Make your feature addition or bug fix. All specs should pass.
3. Add specs for your changes.
4. Commit
5. Send a pull request. Bonus points for topic branches.

## Licence
prawn-gmagick is released under the [MIT Licence](http://choosealicense.com/licenses/mit/)

## Authors
prawn-gmagick is written and maintained by Evan Sharp.
