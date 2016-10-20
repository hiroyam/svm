#include "./util.hpp"

using namespace cc;

#include "./parser.hpp"
#include "./classifier.hpp"

int main(int argc, char *argv[]) {
    try {
        std::vector<vec_t> train_data,  test_data;
        std::vector<int>   train_label, test_label;

        parser::csv::read_iris("./train.csv", train_data, train_label);
        parser::csv::read_iris("./test.csv", test_data, test_label);

        classifier::binary::svm cls;
        cls.fit(train_data, train_label);

        for (size_t i = 0; i < test_data.size(); i++) {
            std::cout << format_str("class= % d predict= % f\n", test_label[i], cls.predict(test_data[i]));
        }
    } catch (const std::exception &e) {
        std::cerr << colorant('y', format_str("error: %s", e.what())) << std::endl;
    }
}

