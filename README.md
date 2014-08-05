Racanhack-bcc
=============

http://racanhack.sourceforge.jp/rhdoc/index.html  
「Racanhack コード解説」を参考に学生時代に作成したものです。  
コメントが無さ過ぎて当時は若かったことを痛感します。  
ANSI Cで作成していたので、現代と合わないところが多々あると思いますが、こういうのもあったな、ということで公開します。  
※ソースはshift-jisです。  


## ファイル構成

* dataフォルダ  
  * /moarea.dat  :エリアデータファイル
  * /monster.csv :モンスターデータファイル
  * /title01.dat :タイトルデータファイル

## 操作

* 基本テンキーで全ての操作が可能
  * 1～9キーで移動
  * 0キーでMENU表示
  * Enterで足踏み（5キーでも可）
* YES:1キー、NO:他キー（Y/y/N/nでも可）
* セーブは階段を降りる毎にその旨が表示される。セーブするとそこでゲームは終了する。

## 各種データ書式

* monster.csv  
一行目にそのカラムの属性名が書いてある。  
なお、"（ダブルコーテーション）区切りにしても問題ない。

    ID:登録ID, NAME:名前, HP:体力, MP:魔力, P:力, D:防御, EXP:経験地, S:行動優先度

* moarea.dat書式  
階数/ボス/レベルモンスター/標準モンスターの順にコロンで分けること  
書式の最後はセミコロンで〆ること  
IDはカンマで区切ること  
なお、monster.h内列挙型のENEMY_MAXが最大種類登録数になる。（現在6）

    F1:                       //そのフロアの階数（F+xxx）  
    990007:                   //ボスmonsterのID（先頭99+ID）  
    010002:                   //LVUPmonsterのID（先頭LV+ID）  
    000001,000002,000003;     //monster.dat(csv)先頭4文字ID（先頭00+ID）  


## 更新履歴

* 2007/10/14：ver1.1.0
  * クリア処理追加（main.c/title.c）
  * 階数制限等を排除
  * 特殊技の制限解除法実装(bossを倒すことで解放)（player.c）
* 2007/10/09：ver1.0.2
  * 壁際での斜め攻撃の禁止（setobject.c）
  * 壁際での斜め移動の禁止（setobject.c）
  * monster.datからmonster.csvへの対応（monster.c）
  * bossの移動表示（monster.c）
  * 他細かいバグ
* 2007/09/23：ver1.0.1
  * ボスモンスターの設定（main.c/monster.c）
* 2007/09/22：ver1.0
  * 初版作成

## 開発環境

Windows XP SP2  
Borland C++ Compiler 5.5.1  
BCC Developer 1.2.21  

