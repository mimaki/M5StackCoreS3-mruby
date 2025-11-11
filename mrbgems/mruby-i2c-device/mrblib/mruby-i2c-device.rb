class I2CDevice
  @i2c = nil
  @addr = -1
  @retry_interval = 0

  def I2CDevice.mruby?
    return RUBY_ENGINE=="mruby"
  end

  def initialize(addr)
    @addr = addr
    @retry_interval = 10 # ms
    if I2CDevice.mruby?
      @i2c = I2C.new @addr,nil
    end
  end

  def read_safe(addr, len)
    if I2CDevice.mruby?
      retry_count = 0
      loop {
        data = @i2c.read(addr, len)
        if data.length == len
          # puts "len=#{len}, data=#{data.inspect}, datalen=#{data.length}"
          return data
        end
        # puts "len=#{len}, data=#{data.inspect}, datalen=#{data.length}, retry=#{retry_count}"
        if retry_count >= 3
          break
        end
        retry_count += 1
        delay @retry_interval
      }
    end
    return "\x00" * len
  end
end
