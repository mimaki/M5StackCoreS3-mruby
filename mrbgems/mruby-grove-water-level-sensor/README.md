# GroveWaterLevelSensor

Grove ⽔位センサ（10cm）を操作するクラス。 

### デバイス情報 
https://www.switch-science.com/products/6282 

### メソッド 

#### new -> GroveWaterLevelSensor
GroveWaterLevelSensorオブジェクトを返します。

#### read -> float 
⽔位（mm）を返します。 

#### 備考 
基板上のセンサの間隔が5mmのため、それ以上細かい計測はできず、計測結果は常に5mmの倍数になります。
