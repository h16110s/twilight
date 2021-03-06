# twilightプロジェクト（プロジェクト名自称）
LittleHOPPERの光る絵本プロジェクトの派生  
読み聞かせ用のデバイスを製作予定

## 使用Arduinoライブラリ
- [DFPlayer-Mini-mp3](https://github.com/DFRobot/DFPlayer-Mini-mp3) 最終的に使用したのはこっちです
- ~~[DFRobotDFPlayerMini](https://github.com/DFRobot/DFRobotDFPlayerMini)~~ ... ArduinoIDEから追加可能
- MFRC522 ... ArduinoIDEから追加
- [nRF24L01+](http://playground.arduino.cc/InterfacingWithHardware/Nrf24L01#.UyByz_l_vh4)
ここからMirfライブラリをDLして追加
- 追加でlibをArduinoのライブラリフォルダにコピーして使用する．

# 内容
　
## lib 
自作のライブラリ．  
- PLED ... 電源LEDのクラスライブラリ
- SceneData ... シーンデータのライブラリ．登録タグの情報などを持つ  
**Arduino IDEの「libraries」にコピーして使用**  
**もしくはそれぞれZip化して「Zip形式のライブラリを追加」**

## Server
主な機能
- RFIDの読み込み
- データの送信  
現在はあらかじめRFIDのUIDを控えて置いてそれぞれのタグを認識する．送信内容についてもあソースコードに組み込む必要があるためある程度不便．

## Server2
こちらもほぼ完成版  
SceneDataクラスを追加してデータを用意することでそのデータを送ってくれる  
送る場合にMirf.payload数の配列を用意する必要があるが，uint8_tでの配列とは異なるため，かなり無理やりなデータ作成方法となっている．  

## Client
主な機能
- データの受信
- 音楽の再生
- モータの振動
- LEDの発行
こっちのソースコードは確か割り込みなしのソースコード（だったはず）  
よってデータを受信して，音楽を鳴らしたり，モータを振動している間は別の受信はしない

## Client2
主な機能はClientと変わらないがMsTimer2ライブラリを用いてデータの再生終了を割り込み処理とした．おかげでデータ再生開始時はデータの受信をできないが割り込み時間が開始すれば割り込まれるまで受信をするようになった．

## Client3
ほとんどClientと変わらない．  
割り込みをやめて送られてきたデータをそのまま反映する．処理時間による時間のずれが問題になると考えられる．

## Client4
ほぼ完成版  
DFPlayerのライブラリを古いものに戻した方がうまく行ったため，戻した．　　
LEDにはMsTime2で割り込み更新を入れた


# 参考にしたサイト
- nRF24L01+ https://ehbtj.com/electronics/nrf24l01/
- nRF24L01+ https://www.electrodragon.com/w/NRF24L01-Mirf
- RFID https://dotstud.io/blog/arduino-use-rfid-reader/
- DFPlayer-Mini-mp3 https://make-muda.net/2016/09/4585/
- DFPlayer-Mini https://blog.techlab-xe.net/archives/5843

# 協力
- [LittleHOPPER](https://www.lilhopper.com/)
