#include "./util.hpp"

using namespace cc;

#include "./parser.hpp"
#include "./classifier.hpp"

int main(int argc, char *argv[]) {
    try {
        std::vector<vec_t> train_data,  test_data;
        std::vector<int>   train_label;

        // アイリスデータを読み込む
        parser::csv::read_iris("./train.csv", train_data, train_label);

        // 学習する
        classifier::binary::svm cls;
        cls.fit(train_data, train_label);

        // 訓練データの範囲を調べる
        float min_x = FLT_MAX;
        float min_y = FLT_MAX;
        float max_x = -FLT_MAX;
        float max_y = -FLT_MAX;
        for (auto v : train_data) {
            min_x = std::min(v[0], min_x);
            min_y = std::min(v[1], min_y);
            max_x = std::max(v[0], max_x);
            max_y = std::max(v[1], max_y);
        }

        // 0.05刻みでテストデータを作る
        float margin = 0.4f;
        for (float x = min_x - margin; x <= max_x + margin; x += 0.05f) {
            for (float y = min_y - margin; y <= max_y + margin; y += 0.05f) {
                vec_t v{x, y};
                test_data.push_back(v);
            }
        }

        // プロット用のデータを出力する
        std::fstream ofs("output", std::ios::out);
        for (auto v : test_data) {
            ofs << format_str("%.2f %.2f %d", v[0], v[1], cls.predict(v) > 0 ? 1 : -1) << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << colorant('y', format_str("error: %s", e.what())) << std::endl;
    }
}

