

## GroveColorSensor 
GROVE I2C カラーセンサ v2.0 を操作するクラス。 

### デバイス情報
https://www.switch-science.com/products/3338 

### メソッド 

#### new -> GroveColorSensor 
GroveColorSensorオブジェクトを返します。 

#### read -> [int, int, int, int] 
カラー値を返します。配列の要素は全て正の整数で、先頭から順にRed,Green,Blue,Clearです。値域は明記されてい ませんが、基板上のLEDを点灯した場合は最⼤で数千に達し、同LEDを消灯した場合は最⼤でも数百のようです。 

**備考**
センサーTCS34725FNのゲインは1x（最⼩）に設定しています。また、積分時間は700ms（最⼤）を設定していま す。したがって、計測値は700msごとに更新されます。それより短い間隔で読み出しても、計測値は変化しません。 
