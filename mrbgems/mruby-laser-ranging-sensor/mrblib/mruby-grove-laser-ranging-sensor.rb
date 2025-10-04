# Laser Ranging Sensor
# https://www.switch-science.com/products/9217
# I2C Address 0x74
# https://wiki.dfrobot.com/Laser_Ranging_Sensor_4m_SKU_SEN0590
class LaserRangingSensor < I2CDevice
  def initialize
    super(0x74)
    if @i2c
      # init - nothing to do
    end
  end

  def read_raw
    if @i2c
      @i2c.write @addr,[0x10, 0xb0]
      sleep 0.05
      @i2c.write @addr,[0x02]
      sleep 0.02
      v = @i2c.read @addr,2
      raw = [v[0].ord, v[1].ord]
    else
      raw = [0,0]
    end
    return raw
  end

  # read 
  def read
    raw = read_raw
    return raw[0]*0x100 + raw[1] + 10
  end
end
