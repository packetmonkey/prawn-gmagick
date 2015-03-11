Token edit XX

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

## Speed
In an unscientific speed test using [this PNG image](https://github.com/prawnpdf/png-testing/blob/master/book.png)
on my MacBook Pro, prawn-gmagick shows a noticeable speed improvement
over Prawn and comparable speed to [prawn-fast-png](https://github.com/amberbit/prawn-fast-png).

```
# Using prawn 0.12.0
$ time ruby png_example.rb
ruby png_example.rb  44.70s user 1.75s system 99% cpu 46.663 total

# Using prawn 0.12.0 with prawn-fast-png
$ time ruby png_example.rb
ruby png_example.rb  1.08s user 0.17s system 99% cpu 1.254 total

# Using prawn 0.13.1
$ time ruby png_example.rb
ruby png_example.rb  5.18s user 0.16s system 97% cpu 5.472 total

# Using prawn 0.13.1 with prawn-gmagick
$ time ruby png_example.rb
ruby png_example.rb  1.17s user 0.20s system 93% cpu 1.463 total

```

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
