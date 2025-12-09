# BLE class for M5Stack 

## BLE class

## Methods

### BLE.new() -> BLE

Creates BLE object.

---

### BLE#send(msg) -> String

Send data via BLE.

#### Parameters

- data
Notification data　(If not a string, use `to_s` before sending)

#### Return
self

---

### BLE#send_temp_humi(tmp, hum)-> String

send Temperature and Humidity via BLE

#### Parameters

- tmp
Temperature

- hum
Humidity



#### Return
self

### BLE#send_color(red, green , blue) -> String

Send RGB color data via BLE.

#### Parameters

- red
red component value
- green
green component value
- blue
blue component value

#### Return
self

### BLE#send_water_level(level) -> String

Send water level value via BLE.

#### Parameters

- level
water level value


#### Return
self

### BLE#send_distance(dist) -> String

Send distance value via BLE.

#### Parameters

- dist
distance value (mm)


#### Return
self

### BLE#send_accel(x, y, z) -> String

Send 3-axis acceleration value via BLE.

#### Parameters

- x
x-axis acceleration

- y
y-axis acceleration

- z
z-axis acceleration


#### Return
self

## License

under the MIT License.
