# mruby-sleep-esp32   [![Build Status](https://travis-ci.org/mimaki/mruby-sleep-ep32.svg?branch=master)](https://travis-ci.org/mimaki/mruby-sleep-esp32)
Kernel.#sleep, delay and delay_us methods for esp32

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

  # ... (snip) ...

  conf.gem :git => 'https://github.com/mimaki/mruby-sleep-esp32'
end
```

## Module functions

- Kernel.sleep(sec) => nil
- Kernel.delay(ms) => nil
- Kernel.delay_us(us) => nil

## example

```ruby
loop {
  puts "Wait for 1 second."
  sleep(1)
  puts "Wait for 500 milliseconds."
  delay(500)
  puts "Wait for 100 microseconds."
  delay_us(100)
}
```

## License

under the MIT License:
- see LICENSE file
