# 自由にデータを扱うクラス
## 概要
- 変数の型に制限されず動的に生成・削除・変更可能なデータ構造
- ファイル入出力が可能 (書式が選べる)
## 使いどころ
- ファイル入出力したいデータがあるけどプログラム書くのが面倒なとき
- 自作インタープリタとかで動的に変数を生成したいとき
## 説明
- Main.cppで使い方を説明
- DataBoxクラスがデータ構造を作る
- DataItemクラスはデータそのもの
- 詳細は各クラスのソースへ
## 備考
- どのデータをどの型として扱うかはユーザー管理
- 誤った操作をした場合例外を出す
- リメイク版→[FreeDataAccess2](https://github.com/Ameyasu/FreeDataAccess2)
