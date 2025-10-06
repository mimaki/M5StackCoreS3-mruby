# M5StackCoreS3-mruby LCD demo

$lcd = LCD.new

#
# LCD.brightness=
#
def brightness_demo
  puts "LCD brightness control demo"
  print "255..."
  LCD.brightness=255
  delay(1000)

  print "64..."
  LCD.brightness=64
  delay(1000)

  print "0..."
  LCD.brightness=0
  delay(1000)

  print "128"
  LCD.brightness=128
end

# delay(2000)
# LCD.clear


#
# LCD.pixel
#
def pixel_demo
  puts "LCD pixel demo"
  10000.times {
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    color = rand(0x1000000)
    LCD.pixel(x, y, color)
  }

  delay(1000)

  10000.times {|i|
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    color = rand(0x1000000)
    if i % 2500 == 0
      $lcd.pixel(x, y, color)
    else
      $lcd.pixel(x, y)
    end
  }
end

delay(2000)
LCD.clear

#
# LCD.line
#
def line_demo
  puts "LCD line demo"

  color = 0
  5.times {
    (0..LCD::WIDTH).each {|x|
      LCD.line(x, 0, LCD::WIDTH-x-1, LCD::HEIGHT-1, color)
      color += 1
    }
    (0..LCD::HEIGHT).each {|y|
      LCD.line(0, LCD::HEIGHT-y-1, LCD::WIDTH-1, y, color)
      color += 1
    }
  }
end

def line_demo2
  puts "LCD line demo"

  1000.times {|i|
    x0 = rand(LCD::WIDTH)
    y0 = rand(LCD::HEIGHT)
    x1 = i%100 == 0 ? rand(LCD::WIDTH) : nil
    y1 = i%100 == 0 ? rand(LCD::HEIGHT) : nil
    color = i%100 == 0 ? rand(0x1000000) : nil
    $lcd.line(x0, y0, x1, y1, color)
    delay(10)
  }
end

# delay(2000)
# LCD.clear

#
# LCD.draw_rect, LCD.fill_rect
#
def rectangle_demo
  puts "LCD rect demo"
  1000.times {
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    w = rand(LCD::WIDTH - x)
    h = rand(LCD::HEIGHT - y)
    color = rand(0x1000000)
    if rand(2) == 0
      LCD.draw_rect(x, y, w, h, color)
    else
      LCD.fill_rect(x, y, w, h, color)
    end
  }
end

def rectangle_demo2
  puts "LCD rect demo2"
  1000.times {|i|
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    w = rand(LCD::WIDTH - x)
    h = rand(LCD::HEIGHT - y)
    color = i % 100 == 0 ? rand(0x1000000) : nil
    fill = (rand(2) == 0)
    $lcd.rect(x, y, w, h, color, fill)
  }
end

# delay(2000)
# LCD.clear

# #
# # LCD.fill_screen
# #
# puts "LCD fill_screen demo"
# colors = [LCD::RED, LCD::GREEN, LCD::BLUE, LCD::YELLOW, LCD::CYAN, LCD::MAGENTA, LCD::WHITE]
# colors.each {|c|
#   LCD.fill_screen(c)
#   delay(500)
# }

# delay(2000)
# LCD.clear

#
# LCD.circle, LCD.fill_circle
#
def circle_demo
  puts "LCD circle demo"
  1000.times {
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    r = rand(30) + 1
    color = rand(0x1000000)
    if rand(2) == 0
      LCD.circle(x, y, r, color)
    else
      LCD.fill_circle(x, y, r, color)
    end
  }
end

def circle_demo2
  puts "LCD circle demo2"
  1000.times {|i|
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    r = rand(30) + 1
    color = i%100 == 0 ? rand(0x1000000) : nil
    fill = (rand(2) == 0)
    $lcd.circle(x, y, r, color, fill)
  }
end

# delay(2000)
# LCD.clear

#
# LCD.triangle, LCD.fill_triangle
#
def triangle_demo
  puts "LCD triangle demo"
  1000.times {
    x0 = rand(LCD::WIDTH)
    y0 = rand(LCD::HEIGHT)
    x1 = rand(LCD::WIDTH)
    y1 = rand(LCD::HEIGHT)
    x2 = rand(LCD::WIDTH)
    y2 = rand(LCD::HEIGHT)
    color = rand(0x1000000)
    if rand(2) == 0
      LCD.triangle(x0, y0, x1, y1, x2, y2, color)
    else
      LCD.fill_triangle(x0, y0, x1, y1, x2, y2, color)
    end
  }
end

def triangle_demo2
  puts "LCD triangle demo2"
  1000.times {|i|
    x0 = rand(LCD::WIDTH)
    y0 = rand(LCD::HEIGHT)
    x1 = rand(LCD::WIDTH)
    y1 = rand(LCD::HEIGHT)
    x2 = rand(LCD::WIDTH)
    y2 = rand(LCD::HEIGHT)
    color = i%100 == 0 ? rand(0x1000000) : nil
    fill = (rand(2) == 0)
    $lcd.triangle(x0, y0, x1, y1, x2, y2, color, fill)
  }
end

# delay(2000)
# LCD.clear

#
# LCD.round_rect, LCD.fill_round_rect
#
def round_rect_demo
  puts "LCD rounded_rect demo"
  1000.times {
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    w = rand(LCD::WIDTH - x)
    h = rand(LCD::HEIGHT - y)
    r = rand(30) + 1
    color = rand(0x1000000)
    if rand(2) == 0
      LCD.round_rect(x, y, w, h, r, color)
    else
      LCD.fill_round_rect(x, y, w, h, r, color)
    end
  }
end

def round_rect_demo2
  puts "LCD rounded_rect demo2"
  1000.times {|i|
    x = rand(LCD::WIDTH)
    y = rand(LCD::HEIGHT)
    w = rand(LCD::WIDTH - x)
    h = rand(LCD::HEIGHT - y)
    r = rand(30) + 1
    color = i%100==0 ? rand(0x1000000) : nil
    fill = (rand(2) == 0)
    $lcd.round_rect(x, y, w, h, r, color, fill)
  }
end

# delay(2000)
# LCD.clear

# #
# # LCD.bitmap
# #
# puts "LCD bitmap demo"
# # 16bit color bitmap (RGB565)
# bmp16 = [
#   0xF800, 0xF800, 0xF800,
#   0x07E0, 0x07E0, 0x07E0,
#   0x001F, 0x001F, 0x001F,
# ]
# bmp = bmp16.pack("S*")  # convert to binary string
# LCD.draw_bitmap(10, 10, 3, 3, bmp, 16)
#
# delay(2000)
# LCD.clear

#
# LCD.draw_char
#
def draw_char_demo
  puts "LCD char/string demo"
  size = 4
  (0..9).each {|i|
    LCD.draw_char(i*size*6, 10, '0'.ord + i, LCD::BLUE, LCD::WHITE, size)
  }
  ('A'..'Z').each_with_index {|ch, i|
    LCD.draw_char((i%13)*size*6, 40+(i/13)*size*8, ch.ord, LCD::GREEN, LCD::YELLOW, size)
  }
  ('a'..'z').each_with_index {|ch, i|
    LCD.draw_char((i%13)*size*6, 100+(i/13)*size*8, ch.ord, LCD::BLACK, LCD::CYAN, size)
  }
end

# delay(2000)
# LCD.clear

#
# LCD.setcursor
#
def cursor_demo
  LCD.text_size=1
  puts "LCD cursor demo"
  LCD.text_size=1
  LCD.set_cursor(0, 1)
  LCD.print "(0,1)"
  LCD.set_cursor(10, 2)
  LCD.print "(10,2)"
  LCD.set_cursor(5, 5)
  LCD.print "(5,5)"
  LCD.text_size=2
  LCD.set_cursor(4, 4)
  LCD.print "(4,4)"
  LCD.set_cursor(6, 6)
  LCD.print "(6,6)"
end

brightness_demo
delay(2000)
LCD.clear
pixel_demo
delay(2000)
LCD.clear
line_demo
delay(2000) 
LCD.clear
line_demo2
delay(2000)
LCD.clear
rectangle_demo
delay(2000)
LCD.clear
rectangle_demo2
delay(2000)
LCD.clear
circle_demo
delay(2000)
LCD.clear
circle_demo2  
delay(2000)
LCD.clear
triangle_demo
delay(2000)
LCD.clear
triangle_demo2
delay(2000)
LCD.clear
round_rect_demo
delay(2000)
LCD.clear
round_rect_demo2
delay(2000)
LCD.clear
draw_char_demo
delay(2000)
LCD.clear
cursor_demo

LCD.text_size=1
puts "\nEnd of lcd demo"
