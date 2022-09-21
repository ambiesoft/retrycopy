# retrycopy
Copy files from a broken HDD

# Explanation
このアプリケーションは正常でなくなったHDDからできるだけファイルを回復することを目的とします。しかし以下のようなHDDは扱いません
This application aims to recover files from a broken HDD. However, the following HDDs are not handled.
* Unrecognized HDD
* HDD with unrecognized disk format or data which file path is not known
* All parts of the file are inaccessible

The following cases are covered.
* Files that can be opened and accessed to some extent but not to some extent

Since normal copying of such a file will stop when a read failure occurs, even if there is an accessible area in the rest of the file.
This application tried to recover as much as possible with following options when such unreadability occurs.
* Reading it again
* Gives up reading and writes zeros for unreadable parts
* Do the above two options automatically

# Warning
* Use of this application cause further damage to your HDD. Please use with caution.
* This application assumes HDD as the read source. It does not take other media into account.

# Available Environments
Windows7 or above, .NET Framework 4.52 or above, Visual Studio 2022 runtime library(Visual Studio 2015 runtime library would be file),、In Windows 10 or above, it is likely already prepared.

# How to install
There is no installer. Please download and extract the compressed file.

# How to uninstall
Delete the unzipped files.

# How to use
* Run **retrycopy.exe**.
* Specify the path of the source file. It is assumed that the file specified here is a file that is hard to read.
* Specify the path of the destination directory. The destination media is assumedto be working properly.
* Click "Start" to begin copying.

# When it failed to read from the file
The read error dialog will appear. The options are as follows.
* Change the size of the buffer. The size of the buffer when reading, but when this error occurs, the file was read up to the point just before the unreadable position regardless of this size.
* Change the number of retries. If this number of attempts is exceeded, the dialog is displayed.
* 読み込みをあきらめてゼロを書き込む。設定されているバッファのサイズの読み込みを飛ばしてそのサイズ分のゼロを書き込み次へ進みます。このときのバッファのサイズはセクターのサイズを最大とする２の累乗がいいと思っています（確信はありません）。
* 上記の作業を自動で行う。手動で行うとエラーが連続しすぎて大変な時は自動化もできます。
* 再試行。もう一度読み込み処理を試みます。

# コピー終了時
どのようにコピーが行われたかはログで確認できます。ダイアログ左下の・・・ボタンから「ログを表示」をクリックしてください。

# ライセンス
このソフトはフリーウェアです。LICENSEファイルをご覧ください。

# コンタクト
- 製作者: Ambiesoft trueff
- メール: <ambiesoft.trueff@gmail.com>
- ウェブサイト: <https://ambiesoft.github.io/webjumper/?target=retrycopy>
- 掲示板: <https://ambiesoft.com/minibbs/minibbs.php>
- 寄付: <https://ambiesoft.github.io/webjumper/?target=donate>
- 開発: <https://github.com/ambiesoft/retrycopy>