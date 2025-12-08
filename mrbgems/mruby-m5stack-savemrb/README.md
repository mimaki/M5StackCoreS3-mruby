savemrb
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
