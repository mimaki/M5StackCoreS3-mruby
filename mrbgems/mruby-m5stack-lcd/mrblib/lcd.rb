class LCD
  # LCD size
  WIDTH = 320
  HEIGHT = 240

  # 16bit(565) colors
  WHITE   = 0xffff
  BLACK   = 0x0000
  RED     = 0xf800
  GREEN   = 0x07e0
  BLUE    = 0x001f
  YELLOW  = 0xffe0
  MAGENTA = 0xf81f
  CYAN    = 0x07ff

  # get 16bit color
  # LCD.rgb(r, g, b) => Fixnum
  #   r: red    (0..0x1f)
  #   g: green  (0..0x3f)
  #   b: blue   (0..0x1f)
  def self.rgb(r, g, b)
    r = 0x1f if r > 0x1f
    g = 0x3f if g > 0x3f
    b = 0x1f if b > 0x1f
    (r << 11) | (g << 5) | b
  end

  # constuctor
  def initialize
    @x = @y = 0
    @fc = LCD::WHITE
    @bc = LCD::BLACK
  end

  # Draw pixel
  # lcd.pixel(x, y, c=nil) => self
  #   x: x axis.
  #   y: y axis.
  #   c: 16bit color (nil: draw with current foreground color)
  def pixel(x, y, c=nil)
    @x, @y = x, y
    @fc = c if c
    LCD.pixel(@x, @y, @fc)
    self
  end

  # Draw line
  # lcd.line(x0, y0, x1=nil, y1=nil, c=nil) => self
  #   x0: x axis #0
  #   y0: y axis #0
  #   x1: x axis #1 (nil: draw line from (@x,@y) to (x0,y0))
  #   y1: y axis #1
  #   c: 16bit color (nil: draw with current foreground color)
  def line(x0, y0, x1=nil, y1=nil, c=nil)
    x1 = x0 unless x1
    y1 = y0 unless y1
    @fc = c if c
    LCD.line(@x, @y, x1, y1, @fc)
    @x, @y = x1, y1
    self
  end

  # rect
  # lcd.rect(x, y, w, h, c=nil, fill=false) => self
  #   x: x axis
  #   y: y axis
  #   w: Width of rectangle
  #   h: Height of rectangle
  #   c: 16bit color (nil: draw with current foreground color)
  #   fill: true: fill a rectangle, false: draw rectangle.
  def rect(x, y, w, h, c=nil, fill=false)
    @x, @y = x, y
    @fc = c if c
    if fill
      LCD.fill_rect(@x, @y, w, h, @fc)
    else
      LCD.draw_rect(@x, @y, w, h, @fc)
    end
    self
  end

  # clear
  # lcd.clear(bc=nil) => self
  #   bc: 16bit color to fill. (nil: fill with current background color)
  def clear(bc=nil)
    @bc = bc if bc
    LCD.fill_screen(@bc)
    LCD.set_cursor(0, 0)
    self
  end

  # circle
  # lcd.circle(x, y, r, c=nil, fill=false) => self
  #   x: x axis
  #   y: y axis
  #   r: radius
  #   c: 16bit color (nil: draw with current foreground color)
  #   fill: true: fill a circle, false: draw circle.
  def circle(x, y, r, c=nil, fill=false)
    @x, @y = x, y
    @fc = c if c
    if fill
      LCD.fill_circle(@x, @y, r, @fc)
    else
      LCD.circle(@x, @y, r, @fc)
    end
    self
  end

  # triangle
  # lcd.triangle(x0, y0, x1, y1, x2, y2, c=nil, fill=false) => self
  #   x0: x axis #0
  #   y0: y axis #0
  #   x1: x axis #1
  #   y1: y axis #1
  #   x2: x axis #2
  #   y2: y axis #2
  #   c: 16bit color (nil: draw with current foreground color)
  #   fill: true: fill a circle, false: draw circle.
  def triangle(x0, y0, x1, y1, x2, y2, c=nil, fill=false)
    @x, @y = x0, y0
    @fc = c if c
    if fill
      LCD.fill_triangle(@x, @y, x1, y1, x2, y2, @fc)
    else
      LCD.triangle(@x, @y, x1, y1, x2, y2, @fc)
    end
    self
  end

  # round_rect
  # lcd.round_rect(x, y, w, h, r, c=nil) => self
  #   x: x axis
  #   y: y axis
  #   w: Width of rectangle
  #   h: Height of rectangle
  #   r: Radius of round
  #   c: 16bit color (nil: draw with current foreground color)
  #   fill: true: fill a round rect, false: draw round rect.
  def round_rect(x, y, w, h, r, c=nil, fill=false)
    @x, @y = x, y
    @fc = c if c
    if fill
      LCD.fill_round_rect(@x, @y, w, h, r, @fc)
    else
      LCD.round_rect(@x, @y, w, h, r, @fc)
    end
    self
  end

  # draw_bitmap
  # lcd.draw_bitmap(x, y, w, h, bmp) => self
  #   x: x axis
  #   y: y axis
  #   w: Width of bitmap
  #   h: Height of bitmap
  #   bmp: Bitmap image data (16bit/pix)
  def draw_bitmap(x, y, w, h, bmp)
    @x, @y = x, y
    LCD.draw_bitmap(@x, @y, w, h, bmp)
    self
  end
end