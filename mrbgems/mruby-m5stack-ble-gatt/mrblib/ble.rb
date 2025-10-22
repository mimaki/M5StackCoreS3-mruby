# BLE class

class BLE
  @buf = ""

  def send_temp_humi(temp, humi)
    notify "TH,#{temp},#{humi}"
  end

  def send_temperature(temp)
    notify "TE,#{temp}"
  end

  def send_humidity(humi)
    notify "HU,#{humi}"
  end

  def send_color(r, g, b)
    notify "CO,#{r},#{g},#{b}"
  end

  def send_water_level(wl)
    notify "WL,#{wl}"
  end

  def send_distance(dist)
    notify "DI,#{dist}"
  end

  def send_accel(x, y, z)
    notify "AC,#{x},#{y},#{z}"
  end

  def send(val)
    val = val.to_s unless val.class == String
    notify val
  end

  def gets
    loop {
      v = read
      if @buf != v
        @buf = v
        return @buf
      end
      delay 1
    }
  end
end
