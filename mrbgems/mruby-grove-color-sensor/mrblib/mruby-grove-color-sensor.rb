# Grove Color Sensor
# https://www.switch-science.com/products/3338
# TCS34725FN
# Arduino https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor_TCS3472
# I2C Address 0x29
class GroveColorSensor < I2CDevice
  # TCS34725
  # Command
  TCS34725_COMMAND_BIT  = 0x80
  TCS34725_AUTO_INC     = 0x20
  # registers
  TCS34725_ENABLE   = 0x00      # Enable
  TCS34725_ATIME    = 0x01      # Integration time
  TCS34725_CONTROL  = 0x0f      # Gain
  TCS34725_ID       = 0x12
  TCS34725_CDATAL   = 0x14
  TCS34725_RDATAL   = 0x16
  TCS34725_GDATAL   = 0x18
  TCS34725_BDATAL   = 0x1a
  # Enable register option
  TCS34725_ENABLE_PON = 0x01 # Power on
  TCS34725_ENABLE_AEN = 0x02 # ADC enable
  # Gain register option
  TCS34725_GAIN_1X    = 0x00 # Gain 1x(min)
  # ATIME register option
  TCS34725_INTEGRATIONTIME_700MS = 0x00 # 700ms(max)

  def initialize
    super(0x29)
    if @i2c
      # Integration time 700ms(max)
      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_ATIME,TCS34725_INTEGRATIONTIME_700MS]
      # Gain 1x(min)
      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_CONTROL,TCS34725_GAIN_1X]
      # Enable : Power on, ADC on
      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_ENABLE,TCS34725_ENABLE_PON]
      sleep 0.01
      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_ENABLE,TCS34725_ENABLE_PON|TCS34725_ENABLE_AEN]
    end
  end

  # read_raw() -> [Red,Green,Blue,Clear]
  def read_raw
    r,g,b,c = 0,0,0,0
    if @i2c
      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_AUTO_INC|TCS34725_RDATAL]
      v = read_safe(@addr, 2)
      if v.length == 2
        r = v[0].ord + v[1].ord*256
      end

      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_AUTO_INC|TCS34725_GDATAL]
      v = read_safe(@addr, 2)
      if v.length == 2
        g = v[0].ord + v[1].ord*256
      end

      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_AUTO_INC|TCS34725_BDATAL]
      v = read_safe(@addr, 2)
      if v.length == 2
        b = v[0].ord + v[1].ord*256
      end

      @i2c.write @addr,[TCS34725_COMMAND_BIT|TCS34725_AUTO_INC|TCS34725_CDATAL]
      v = read_safe(@addr, 2)
      if v.length == 2
        c = v[0].ord + v[1].ord*256
      end

    #   return [r,g,b,c]
    # else
    #   return [100,200,300,400]
    end
    return [r,g,b,c]
  end

  # read() -> [Red,Green,Blue,Clear]
  def read
    return read_raw
  end
end
