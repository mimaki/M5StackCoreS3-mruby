# M5StackCoreS3-mruby

## はじめに

M5StackCoreS3-mruby は、[M5Stack CoreS3](https://docs.m5stack.com/ja/core/CoreS3) 上で [mruby](https://github.com/mruby/mruby) を動作させるためのサンプルプロジェクトです。
ESP-IDF（v5.5）と M5Unified をベースにしており、CoreS3 の基本的なデバイス機能（LCD、SDカードアクセスなど）へ Ruby スクリプトからアクセスできる環境を提供します。

本プロジェクトは以下を目的としています。
- M5Stack CoreS3 上での mruby 実行環境の構築方法を示す
- C/C++（ESP-IDF）による機能を Ruby から呼び出す仕組みの例を提供
- CoreS3 を用いたスクリプトベース開発の足がかり

## M5StackCoreS3-mruby の概要

本リポジトリでは以下を提供します。
- mruby のビルド設定（mrbgems の追加など）
- ESP-IDF + M5Unified の環境構築
- mruby インタプリタの組み込みコード
- LCDやシリアル出力などの簡易APIをmruby側にバインディングした例

## 対応環境
- M5Stack CoreS3 / CoreS3 SE
- ESP-IDF 5.5
- macOS / Linux / Windows（ESP-IDF の公式対応環境）

## ビルド手順

以下では、ESP-IDF 5.5 を前提に手順を示します。

1. ESP-IDF のセットアップ

ESP-IDF 5.5 をインストールし、環境変数を有効化します。
```sh
. $HOME/esp/esp-idf/export.sh
```

2. リポジトリの取得
```sh
git clone --recursive https://github.com/mruby-Forum/M5StackCoreS3-mruby.git
cd M5StackCoreS3-mruby
```

3. ESP-IDF プロジェクトのビルド
```sh
idf.py set-target esp32s3
idf.py build
```

4. M5Stack CoreS3への書き込み（Flash）

- M5Stack CoreS3をUSBで接続します。
- CoreS3のリセットボタンを長押します。（ダウンロードモードに移行します）
- 以下を実行してファームウェアを更新します。

```sh
idf.py flash
```

## 利用手順

macOSでの利用手順を示します。
Windows環境でもご利用頂けますが、PC側で実行する際のパス名の仕様が異なりますので、その点はご注意ください。

### REPL環境の利用

CoolTermなどのターミナルソフトでM5Stack CoreS3に接続します。
ターミナルの設定は以下の通りです。

|項目|設定値|
|:--|:-:|
|Baudrate|115200|
|Data bits|8|
|Parity|None|
|Stop bits|1|
|Enter Key|LF|

M5Stack CoreS3をリセットすると、REPL(mirb)モードで起動します。
ターミナルからの対話形式で、mrubyスクリプトを実行することができます。

### mrubyアプリの実行

M5StackCoreS3-mrubyでは、micro SDカードにmrubyアプリケーションを格納して、メニューからアプリケーションを選択することでmrubyアプリケーションを実行することができます。

#### 事前準備
- micro SDカードをM5Stack CoreS3に装着します
- M5Stack CoreS3をPCにUSB接続して、ターミナルソフト(CoolTerm推奨)で接続します。

#### mrubyアプリケーションのコンパイル

M5StackCoreS3-mrubyでmrubyアプリケーションを実行するには、mrubyコンパイラ(mrbc)によって、Rubyスクリプトをmrubyバイナリ(mrbファイル)にコンパイルする必要があります。
※ 現状、Rubyスクリプトのまま実行する機能はサポートしていません。

- `mrbc` は以下のフォルダに格納されています  
`components/mruby_component/mruby/bin`  
コンパイル実行例)  
M5StackCoreS3-mrubyのフォルダ内に `foo.rb` を作成して以下のコマンドを実行すると、mrubyバイナリ `foo.mrb` が作成されます。
`components/mruby_component/mruby/bin/mrbc foo.rb`

#### mrubyアプリケーションの転送

コンパイルしたmrubyバイナリ(mrbファイル)をM5Stackに転送します。

- M5StackCoreS3-mrubyのREPL(mirb)モードから、以下のコマンドを実行します。  
`savemrb "AppName"`
  - `AppName` は任意のアプリケーション名 (英数字8文字以内)を指定します。
  - `AppName` には拡張子`.mrb`は不要です。
- `savemrb` を実行するとM5Stack CoreS3側はアプリケーション転送待ち状態になります。
- CoolTermの `Connection > Send File(s)...` を選択します。
- `Select Transfer Protocol`ダイアログが表示されるので、`XMODEM`を選択して`Select File...`をクリックします。
- ファイル選択ダイアログが表示されるので、M5Stackに転送するmrubyアプリケーションバイナリ(上記の例では`foo.mrb`)を選択します。
- ファイル転送が行われ、転送完了後にダイアログが表示されます。（M5Stack CoreS3はリセットされます）

#### メニューからのmrubyアプリケーション実行

- REPL(mirb)モードで以下のコマンドを実行します。  
`mrbmenu`
- 転送済みのmrubyアプリケーションが一覧表示されるので、実行したいアプリケーションの番号を入力します。
- 実行確認メッセージが表示されるので `y` を入力します。
- M5Stack CoreS3がリセットされて再起動後に指定したアプリケーションが実行されます。

#### 補足
- アプリケーションの実行が終了するとREPL(mirb)モードに移行します。
- 無限ループや処理時間が長いアプリケーションを終了させたい場合は、M5Stack本体のリセットボタンを押下してください。

### mrubyアプリの自動実行 

micro SDカードのルートディレクトリに `autorun.mrb` のファイル名でmrubyアプリケーションを配置することで、M5StackCore3-mruby起動時にmrubyアプリケーションを自動実行することができます。

- `autorun.mrb` を作成するには mrubyコンパイラ `mrbc` が必要です  
M5StackCoreS3-mrubyのフォルダ内に `foo.rb` を作成してコンパイルする場合は以下のように実行します。
```sh
components/mruby_component/mruby/bin/mrbc -o autorun.mrb foo.rb
```

- `autorun.mrb` のアプリケーションが終了すると、REPL(mirb)モードに移行します。

#### 注意
- `mrbmenu` からmrubyアプリケーション実行する場合は、mirco SDの`autorun.mrb`を削除してください。

## M5StackCoreS3-mrubyで利用可能なライブラリ

M5StackCoreS3-mrubyに搭載している拡張ライブラリ(mrbgems)を以下に示します。

|mrbgem|概要|
|:--|:--|
|[mruby-stdio](https://github.com/mruby-Forum/mruby-stdio)|標準入出力ライブラリ|
|[mruby-m5stack-gpio](mrbgems/mruby-m5stack-gpio)|GPIO(汎用IO)ライブラリ|
|[mruby-m5stack-pwm](mrbgems/mruby-m5stack-pwm)|PWM制御ライブラリ|
|[mruby-stdio-m5stack](mrbgems/mruby-stdio-m5stack)|M5Stack用標準入出力ライブラリ|
|[mruby-sleep-esp32](mrbgems/mruby-sleep-esp32)|sleep, delayメソッド|
|[mruby-m5stack-i2c](mrbgems/mruby-m5stack-i2c)|I2C制御ライブラリ|
|[mruby-m5stack-ble-gatt](mrbgems/mruby-m5stack-ble-gatt)|M5Stack BLE GATTライブラリ|
|[mruby-m5stack-lcd](mrbgems/mruby-m5stack-lcd)|M5Stack CoreS3 LCDライブラリ|
|[mruby-i2c-device](mrbgems/mruby-i2c-device)|I2Cデバイスライブラリ|
|[mruby-grove-color-sensor](mrbgems/mruby-grove-color-sensor)|GROVEカラーセンサライブラリ|
|[mruby-grove-temp-humi-sensor](mrbgems/mruby-grove-temp-humi-sensor)|GROVE温湿度センサライブラリ|
|[mruby-grove-water-level-sensor](mrbgems/mruby-grove-water-level-sensor)|GROVE水位センサライブラリ|
|[mruby-laser-ranging-sensor](mrbgems/mruby-laser-ranging-sensor)|レーザ距離センサライブラリ|
|[mruby-haext-accel](mrbgems/mruby-haext-accel)|加速度センサ(M5Stack CoreS3内蔵)ライブラリ|
|[mruby-m5stack-savemrb](mrbgems/mruby-m5stack-savemrb)|mrubyファイル転送|
|[mruby-m5stack-mrb](mrbgems/mruby-m5stack-mrb)|mrubyアプリメニュー|


※ 機能の詳細は準備中です。

## License

MIT License.
