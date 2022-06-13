# esp32-Wi-Fi-test

* freeRTOSでWi-Fiで接続し、WebAPIがたたけるかどうか確認する
* 結論からいえば、task化はできたが、Wi-Fiの監視とHttpRequestは分離することができなかった
* メッセージQueueを2つつくり、別タスクから要求をなげ、返りをもう一つのメッセージQueueでまてば、WebApiの要求応答が実現可能
* 最低限実装はできたので、いったんこのまま進める
* https://forum.arduino.cc/t/connect-to-wifi-with-task-esp32-freertos/683438