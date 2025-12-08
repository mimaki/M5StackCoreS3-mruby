# GroveTempHumiSensor

GROVE 温湿度センサ（SHT31搭載）を操作するクラス。 

### デバイス情報 
https://www.switch-science.com/products/8737 

### メソッド

#### new -> GroveTempHumiSensor 
GroveTempHumiSensorオブジェクトを返します。 

#### read -> [float, float]
温度および湿度を返します。
要素は先頭から温度（℃）、湿度（%）です。 

#### 備考 
計測精度は3段階のうち最も⾼再現性の設定にしています。この場合、計測時間は12.5msから15msです。
本クラス のread()ではもっと⻑い0.1秒間待っています。
