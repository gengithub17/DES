# DES(暗号方式)
## 中身
- src/
  - main.c : メイン実行関数
  - Key.c : 鍵管理関数
  - Round.c : 各ラウンド実行関数
  - Trans.c : 初期転置および最終転置
  - Values.c : 2進数出力関数および用いる数値定義
- lib/
  - Constant.h : 定数定義
- options.h : マクロ宣言
- .vscode/c_cpp_properties.json : vscode用設定ファイル
  - Mac用なので、環境に合わせてnameを"Win", "Linux"に変更
## 実行方法
### Makefileより
- makeコマンドでコンパイル&実行まで
- make fileでdata.txtに出力(codeコマンドでvscode表示まで)
- make cleanで実行ファイルと出力ファイルを削除
### main.c
- 通常モード
  - options.hではマクロ宣言不要
  - encrypt, decrypt関数で暗号化および復号
- DEBUG_WHOLE
  - 各ラウンドの関数出力値および鍵の値を出力
  - options.hでDEBUG_WHOLEを宣言
  - main.cでEncDec_Debug実行
- TEST_MODE
  - DEBUG_TRANS, DEBUG_ROUND, DEBUG_ROUND_F, DEBUG_KEY
  - などと合わせて宣言することで、
  - 関数の入出力テストが行える
  - テスト用関数は各ソースコード参照