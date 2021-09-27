# graphics-sample

描画系のサンプルソース

## ビルド

### Windows

`cmake-gui` を起動して、以下の設定をします。

- Where is the source code：${クローンしたフォルダ}
- Where to build the binaries：${クローンしたフォルダ}/build

`Configure` をクリックします。
ビルドしたいプラットフォームを選択します。

`BUILD_LIBRARY` にチェックを入れて、`Generate` をクリックします。

`Open Project` をクリックしてVisual Studioを起動します。

Visual Studio上でビルドを実行します。

## freetype01

freetypeを使って、文字列からビットマップ画像を作成するサンプルコードです。

要解析。。。

- 特定の文字列だとリセットします。
- 斜体にするとおかしい結果になります。
