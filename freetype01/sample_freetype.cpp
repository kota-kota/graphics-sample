#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftsynth.h>
#include <freetype/ftglyph.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace {
    FT_Library  ft_library = nullptr;   //!< FreeTypeインスタンスハンドル
    FT_Face     ft_face = nullptr;      //!< FreeTypeフェイスオブジェクトハンドル
}

namespace {
    class Text {
    public:
        std::uint32_t   size_;
        std::uint32_t   color_;     //RGBA
        bool            bold_;
        bool            italic_;
        std::wstring    str_;
    };
    std::vector<Text> SAMPLE_TEXTS = {
        { 32u, 0xFF0000FFu, false, false, L"abcdefghijklmnopqrstuvwxyz" },
        { 16u, 0xFFFF00FFu, false, false, L"さんぷる　サンプル　ｻﾝﾌﾟﾙ" },
        { 48u, 0x0000FFFFu, true,  false , L"太字Bold_" },
        //{ 32u, 0xFF0000FFu, false, false, L"m(_ ^ _)m" },     // ここを有効にするとリセット
    };
}

class Bitmap {
public:
    std::uint32_t   size_;
    std::uint8_t*   image_;
};
Bitmap make_text_bitmap(const Text& text);
Bitmap make_bitmap(const std::uint8_t* image_alpha, const std::uint32_t color, const std::int32_t width, const std::int32_t height);

int main() {
    FT_Error    fterr = 0;

    // FreeTypeインスタンスハンドルの初期化
    fterr = FT_Init_FreeType(&ft_library);
    if(fterr != 0) {
        std::cout << "* FT_Init_FreeType() .. NG (" << fterr << ")" << std::endl;
        return 1;
    }
    std::cout << "* FT_Init_FreeType() .. OK" << std::endl;

    // FreeTypeフェイスオブジェクトハンドルの生成
    fterr = FT_New_Face(ft_library, "C:\\Windows\\Fonts\\meiryo.ttc", 0, &ft_face);
    //fterr = FT_New_Face(ft_library, "C:\\Windows\\Fonts\\msgothic.ttc", 0, &ft_face);
    if(fterr != 0) {
        std::cout << "* FT_New_Face() .. NG (" << fterr << ")" << std::endl;
        (void)FT_Done_FreeType(ft_library);
        return 1;
    }
    std::cout << "* FT_New_Face() .. OK" << std::endl;

    for (size_t i = 0; i < SAMPLE_TEXTS.size(); ++i) {
        Bitmap bitmap = make_text_bitmap(SAMPLE_TEXTS[i]);
        std::string filename = "text" + std::to_string(i) + ".bmp";
        std::ofstream fout;
        fout.open(filename, std::ios::out|std::ios::binary);
        fout.write((char*)bitmap.image_, bitmap.size_);
        delete[] bitmap.image_;
    }

    (void)FT_Done_Face(ft_face);
    (void)FT_Done_FreeType(ft_library);

    return 0;
}



Bitmap make_text_bitmap(const Text& text) {
    // フォント寸法情報
    struct FontMetrics {
        std::int32_t    width_;     // フォント幅[pixel]
        std::int32_t    height_;    // フォント高さ[pixel]
        std::int32_t    offsetX_;   // グリフ原点(0,0)からグリフイメージの左端までの水平方向オフセット[pixel]
        std::int32_t    offsetY_;   // グリフ原点(0,0)からグリフイメージの上端までの垂直方向オフセット[pixel]
        std::int32_t    nextX_;     // 次グリフへの水平方向オフセット[pixel]
        std::int32_t    nextY_;     // 次グリフへの垂直方向オフセット[pixel]
        std::int32_t    kerningX_;  // 水平方向カーニング
        std::int32_t    kerningY_;  // 垂直方向カーニング
    };
    // フォントグリフ
    struct FontGlyph {
        FT_UInt         index_;     // グリフインデックス
        FT_Glyph        image_;     // グリフイメージ
        FontMetrics     metrics_;   // 寸法情報
    };

    // テキスト文字列のフォントグリフ保持用
    const std::int32_t MAX_GLYPHS = 32;
    FontGlyph glyphs[MAX_GLYPHS];
    std::int32_t numGlyphs = 0;

    // 文字列のバウンディングボックス
    FT_BBox stringBBox = { 0 };

    // フォントサイズ設定
    FT_Set_Char_Size(ft_face, text.size_ * 64, 0, 96, 0);

    // 文字列の長さ分ループ
    for (std::size_t i = 0; i < text.str_.size(); i++) {
        // 処理対象文字
        const std::uint16_t c = static_cast<std::uint16_t>(text.str_[i]);

        // 処理対象文字のグリフ格納用
        FontGlyph* glyph = &glyphs[numGlyphs];

        // グリフインデックスを取得
        glyph->index_ = FT_Get_Char_Index(ft_face, c);

        // グリフをロード
        FT_Load_Glyph(ft_face, glyph->index_, FT_LOAD_DEFAULT);

        // ボールド加工
        if (text.bold_) {
            FT_GlyphSlot_Embolden(ft_face->glyph);
        }
        // イタリック加工
        if (text.italic_) {
            FT_GlyphSlot_Oblique(ft_face->glyph);
        }

        // グリフを描画
        FT_Get_Glyph(ft_face->glyph, &glyph->image_);
        FT_Glyph_To_Bitmap(&glyph->image_, FT_RENDER_MODE_NORMAL, nullptr, 1);

        // 寸法情報を取得
        FT_BitmapGlyph bit = (FT_BitmapGlyph)glyph->image_;
        glyph->metrics_.width_ = bit->bitmap.width;
        glyph->metrics_.height_ = bit->bitmap.rows;
        glyph->metrics_.offsetX_ = bit->left;
        glyph->metrics_.offsetY_ = bit->top;
        glyph->metrics_.nextX_ = ft_face->glyph->advance.x >> 6;
        glyph->metrics_.nextY_ = ft_face->glyph->advance.y >> 6;

        // 処理対象文字のバウンディングボックスを取得
        FT_BBox bbox;
        FT_Glyph_Get_CBox(glyph->image_, ft_glyph_bbox_pixels, &bbox);

        if (i == 0) {
            stringBBox.xMin = 0;
            stringBBox.xMax = glyph->metrics_.nextX_;
            stringBBox.yMin = bbox.yMin;
            stringBBox.yMax = bbox.yMax;
        }
        else {
            stringBBox.xMin = 0;
            stringBBox.xMax += glyph->metrics_.nextX_;
            if (bbox.yMin < stringBBox.yMin) { stringBBox.yMin = bbox.yMin; }
            if (bbox.yMax > stringBBox.yMax) { stringBBox.yMax = bbox.yMax; }
        }

        // 次文字へ
        numGlyphs++;
    }

    // 文字列の幅高さ
    const std::int32_t stringW = stringBBox.xMax - stringBBox.xMin;
    const std::int32_t stringH = stringBBox.yMax - stringBBox.yMin;

    // 文字列画像の領域を確保
    const std::int32_t imagesize = stringW * stringH;
    std::uint8_t* image_alpha = new std::uint8_t[imagesize];
    memset(image_alpha, 0, imagesize);

    // 文字列のアルファ画像を生成
    FT_Vector pen = { 0, stringBBox.yMax };
    for (std::int32_t i = 0; i < numGlyphs; i++) {
        // 処理対象文字のグリフを取得
        FontGlyph* glyph = &glyphs[i];
        FT_BitmapGlyph bit = (FT_BitmapGlyph)glyph->image_;

        const std::int32_t xoffset = pen.x + glyph->metrics_.offsetX_;
        const std::int32_t yoffset = pen.y - glyph->metrics_.offsetY_;
        std::int32_t readOffset = 0;
        std::int32_t writeOffset = xoffset + (yoffset * stringW);
        for (std::int32_t h = 0; h < glyph->metrics_.height_; h++) {
            (void)memcpy_s(&image_alpha[writeOffset], stringW, bit->bitmap.buffer + readOffset, glyph->metrics_.width_);
            readOffset += glyph->metrics_.width_;
            writeOffset += stringW;
        }

        pen.x += glyph->metrics_.nextX_;

        // グリフイメージ破棄
        FT_Done_Glyph(glyph->image_);
    }

    Bitmap bitmap = make_bitmap(image_alpha, text.color_, stringW, stringH);
    delete[] image_alpha;
    return bitmap;
}

Bitmap make_bitmap(const std::uint8_t* image_alpha, const std::uint32_t color, const std::int32_t width, const std::int32_t height) {
    const std::int32_t BFH_HEADERSIZE = 14; // Bitmapファイルヘッダ(Windows,OS/2共通)
    const std::int32_t BIH_HEADERSIZE = 40; // Bitmap情報ヘッダ(Windows)

    // Bitmapファイルヘッダ(Windows,OS/2共通)
    std::uint8_t bFileHeader[BFH_HEADERSIZE];
    // ファイルタイプ
    bFileHeader[0] = 'B'; bFileHeader[1] = 'M';
    // ファイルサイズ
    std::int32_t bfSize = BFH_HEADERSIZE + BIH_HEADERSIZE + (width * height * 4);
    memcpy(&bFileHeader[2], &bfSize, 4);
    // 予約領域1
    memset(&bFileHeader[6], 0, 2);
    // 予約領域2
    memset(&bFileHeader[8], 0, 2);
    // ファイル先頭から画像データまでのオフセット
    std::int32_t bfOffBits = BFH_HEADERSIZE + BIH_HEADERSIZE;
    memcpy(&bFileHeader[10], &bfOffBits, 4);

    // Bitmap情報ヘッダ(Windows)
    std::uint8_t bInfoHeader[BIH_HEADERSIZE];
    // 情報ヘッダサイズ
    std::int32_t biSize = BIH_HEADERSIZE;
    memcpy(&bInfoHeader[0], &biSize, 4);
    // 画像の幅
    std::int32_t biWidth = width;
    memcpy(&bInfoHeader[4], &biWidth, 4);
    // 画像の高さ
    std::int32_t biHeight = height;
    memcpy(&bInfoHeader[8], &biHeight, 4);
    // プレーン数
    std::uint16_t biPlanes = 1;
    memcpy(&bInfoHeader[12], &biPlanes, 2);
    // 色ビット数
    std::int16_t biBitCount = 32;
    memcpy(&bInfoHeader[14], &biBitCount, 2);
    // 圧縮形式
    std::int32_t biCompression = 0;
    memcpy(&bInfoHeader[16], &biCompression, 4);
    // 画像データサイズ
    std::int32_t biSizeImage = width * height * 4;
    memcpy(&bInfoHeader[20], &biSizeImage, 4);
    // 水平解像度
    memset(&bInfoHeader[24], 0, 4);
    // 垂直解像度
    memset(&bInfoHeader[28], 0, 4);
    // 格納パレット数
    memset(&bInfoHeader[32], 0, 4);
    // 重要色数
    memset(&bInfoHeader[36], 0, 4);

    std::uint8_t r = static_cast<std::uint8_t>((color & 0xFF000000) >> 24);
    std::uint8_t g = static_cast<std::uint8_t>((color & 0x00FF0000) >> 16);
    std::uint8_t b = static_cast<std::uint8_t>((color & 0x0000FF00) >> 8);
    std::uint8_t a = static_cast<std::uint8_t>(color & 0x000000FF);

    std::uint8_t* image_rgba = new std::uint8_t[biSizeImage];
    std::int32_t readOffset = 0;
    for (std::int32_t h = 0; h < height; h++) {
        std::int32_t writeIndex = (height - h - 1) * width * 4;
        for (std::int32_t w = 0; w < width; w++) {
            double alpha = static_cast<double>(*(image_alpha + readOffset)) / 255.0;
            //出力データへBGRA値を設定
            *(image_rgba + writeIndex + 0) = static_cast<std::uint8_t>(alpha * b);  //青
            *(image_rgba + writeIndex + 1) = static_cast<std::uint8_t>(alpha * g);  //緑
            *(image_rgba + writeIndex + 2) = static_cast<std::uint8_t>(alpha * r);  //赤
            *(image_rgba + writeIndex + 3) = a; //アルファ

            //読み込み位置を更新
            readOffset += 1;
            //書き込み位置を更新
            writeIndex += 4;
        }
    }
    Bitmap bitmap;
    bitmap.size_ = bfSize;
    bitmap.image_ = new std::uint8_t[bfSize];
    memcpy_s(&bitmap.image_[0], BFH_HEADERSIZE , &bFileHeader[0], BFH_HEADERSIZE);
    memcpy_s(&bitmap.image_[BFH_HEADERSIZE], BIH_HEADERSIZE , &bInfoHeader[0], BIH_HEADERSIZE);
    memcpy_s(&bitmap.image_[bfOffBits], biSizeImage , &image_rgba[0], biSizeImage);
    delete[] image_rgba;

    return bitmap;
}