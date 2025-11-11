# Water Level Sensor
# https://www.switch-science.com/products/6282
# I2C Address 0x78,0x77 (High 12, Low 8)
# https://wiki.seeedstudio.com/Grove-Water-Level-Sensor/#software
class GroveWaterLevelSensor < I2CDevice
  THRESHOLD = 100

  def initialize
    super(0x78)
    if @i2c
      # init - nothing to do
    end
  end

  def read_raw
    touch_lo = Array.new(8, 0)
    touch_hi = Array.new(12, 0)
    if @i2c
      lo = read_safe(@addr-1, 8)
      hi = read_safe(@addr, 12)
      if lo.length == 8 && hi.length == 12
        touch_lo = []
        touch_hi = []
        lo.each_char {|c| touch_lo.push( c.ord ) }
        hi.each_char {|c| touch_hi.push( c.ord ) }
        touch_hi = [0,0,0,0,0,0,0,0,0,0,0,0]
      end
    # else
    #   touch_lo = [255,255,255,255,255,255,255,255]
    #   touch_hi = [255,255,0,0,0,0,0,0,0,0]
    end
    raw = touch_lo + touch_hi
    return raw
  end

  # read 
  def read
    # count touches over THRESHOLD
    raw = read_raw
    touch = 0
    raw.each do |v|
      if v > THRESHOLD
        touch += 1
      end
    end
    # 20 sensors on board, so 5% / touch
    return touch.to_f*5.0
  end
end
