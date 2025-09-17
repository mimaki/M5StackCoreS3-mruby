# mruby-stdio-m5stack   [![Build Status](https://travis-ci.org/mimaki/mruby-stdio-m5stakc.svg?branch=master)](https://travis-ci.org/mimaki/mruby-stdio-m5stack)
STDOUT._putc and STDIN._getc for M5Stack

## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

  # ... (snip) ...

  conf.gem :git => 'https://github.com/mimaki/mruby-stdio'
  conf.gem :git => 'https://github.com/mimaki/mruby-stdio-m5stack'
end
```

## example
```ruby
$stdout.print '=> '
$stdout.puts $stdin.gets.chomp
```

## License
under the MIT License:
- see LICENSE file
