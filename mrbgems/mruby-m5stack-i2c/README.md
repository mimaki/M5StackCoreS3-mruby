# I2C class for M5Stack Basic & Gray

## I2C class

## Methods

### I2C.new() -> I2C

Creates I2C object.

---

### I2C#read(addr, len, *outdata) -> String

Read data from I2C.

#### Parameters

- addr  
Slave address of I2C device.

- len  
Data length for read.

- *outdata

Output data before read.

#### Return

Array of read data.

---

### I2C#write(addr, *outdata, stop=true) -> bool

#### Parameters

- addr  
Slave address of I2C device.

- *outdata

Output data.

- stop

This allows one controller device to send multiple transmissions while in control.

#### Return

Result of write. (true: Success, false: Failure)

## License

under the MIT License.
