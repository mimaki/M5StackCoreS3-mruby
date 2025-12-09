# mruby-lcd-m5stack   [![Build Status](https://travis-ci.org/mimaki/mruby-lcd-m5stakc.svg?branch=master)](https://travis-ci.org/mimaki/mruby-lcd-m5stack)
LCD class for M5Stack

## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

  # ... (snip) ...

  conf.gem :git => 'https://github.com/mimaki/mruby-lcd-m5stack'
end
```

## Constants

### Screen Size

- WIDTH
- HEIGHT

### Colors

- WHITE
- BLACK
- RED
- GREEN
- BLUE
- YELLOW
- MAGENTA
- CYAN

## Singleton methods

- LCD.pixel(x, y, color) => self
- LCD.line(x0, y0, x1, y1, color) => self
- LCD.fill_rect(x, y, width, height, color) => self
- LCD.fill_screen(color) => self
- LCD.clear => self
- LCD.circle(x, y, r, color) => self
- LCD.fill_circle(x, y, r, color) => self
- LCD.triangle(x0, y0, x1, y1, x2, y2, color) => self
- LCD.fill_triangle(x0, y0, x1, y1, x2, y2, color) => self
- LCD.round_rect(x, y, width, height, radius, color) => self
- LCD.fill_round_rect(x, y, width, height, radius, color) => self
- LCD.draw_bitmap(x, y, width, height, bitmap) => self
- LCD.rgb(r, g, b) => Fixnum

## Instance methods

- lcd.pixel(x, y, color=nil) => self
- lcd.line(x0, y0, x1=nil, y1=nil, color=nil) => self
- lcd.rect(x, y, w, h, color=nil, fill=false) => self
- lcd.clear(bcolor=nil) => false
- lcd.circle(x, y, r, color=nil, fill=false) => false
- lcd.triangle(x0, y0, x1, y1, x2, y2, color=nil, fill=false) => self
- lcd.round_rect(x, y, w, h, r, color=nil, fill=false) => self

## example
```ruby
lcd = LCD.new
lcd.clear
x = 0
r = 0
loop do
  lcd.rect x, 0, 1, LCD::HEIGHT, LCD.rgb(r, 0, 0)
  x = (x + 1) % LCD::WIDTH
  r = (r + 1) & 0x1f
end
```

## License
under the MIT License:
- see LICENSE file
