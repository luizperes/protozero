
#include <test.hpp>

namespace TestComplex {

enum class Test : protozero::pbf_tag_type {
    required_fixed32_f      = 1,
    optional_int64_i        = 2,
    optional_int64_j        = 3,
    required_Sub_submessage = 5,
    optional_string_s       = 8,
    repeated_uint32_u       = 4,
    packed_sint32_d         = 7
};

enum class Sub : protozero::pbf_tag_type {
    required_string_s = 1
};

} // end namespace TestComplex

TEST_CASE("read complex data using pbf_reader") {

    SECTION("minimal") {
        const std::string buffer = load_data("complex/data-minimal");

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

    SECTION("some") {
        const std::string buffer = load_data("complex/data-some");

        protozero::pbf_reader item2(buffer);
        protozero::pbf_reader item;
        using std::swap;
        swap(item, item2);

        uint32_t sum_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 4: {
                    sum_of_u += item.get_uint32();
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(sum_of_u == 66);
    }

    SECTION("all") {
        const std::string buffer = load_data("complex/data-all");

        protozero::pbf_reader item(buffer);

        int number_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 3: {
                    REQUIRE(item.get_int64() == 555555555LL);
                    break;
                }
                case 4: {
                    item.skip();
                    ++number_of_u;
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case 7: {
                    const auto pi = item.get_packed_sint32();
                    int32_t sum = 0;
                    for (auto val : pi) {
                        sum += val;
                    }
                    REQUIRE(sum == 5);
                    break;
                }
                case 8: {
                    REQUIRE(item.get_string() == "optionalstring");
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(number_of_u == 5);
    }

    SECTION("skip everything") {
        const std::string buffer = load_data("complex/data-all");

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 7:
                case 8:
                    item.skip();
                    break;
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

}

TEST_CASE("read complex data using pbf_message") {

    SECTION("minimal") {
        const std::string buffer = load_data("complex/data-minimal");

        protozero::pbf_message<TestComplex::Test> item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case TestComplex::Test::required_fixed32_f: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case TestComplex::Test::required_Sub_submessage: {
                    protozero::pbf_message<TestComplex::Sub> subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

    SECTION("some") {
        const std::string buffer = load_data("complex/data-some");

        protozero::pbf_message<TestComplex::Test> item2(buffer);
        protozero::pbf_message<TestComplex::Test> item;
        using std::swap;
        swap(item, item2);

        uint32_t sum_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case TestComplex::Test::required_fixed32_f: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case TestComplex::Test::optional_int64_i: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case TestComplex::Test::repeated_uint32_u: {
                    sum_of_u += item.get_uint32();
                    break;
                }
                case TestComplex::Test::required_Sub_submessage: {
                    protozero::pbf_message<TestComplex::Sub> subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(sum_of_u == 66);
    }

    SECTION("all") {
        const std::string buffer = load_data("complex/data-all");

        protozero::pbf_message<TestComplex::Test> item(buffer);

        int number_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case TestComplex::Test::required_fixed32_f: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case TestComplex::Test::optional_int64_i: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case TestComplex::Test::optional_int64_j: {
                    REQUIRE(item.get_int64() == 555555555LL);
                    break;
                }
                case TestComplex::Test::repeated_uint32_u: {
                    item.skip();
                    ++number_of_u;
                    break;
                }
                case TestComplex::Test::required_Sub_submessage: {
                    protozero::pbf_message<TestComplex::Sub> subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case TestComplex::Test::packed_sint32_d: {
                    const auto pi = item.get_packed_sint32();
                    int32_t sum = 0;
                    for (auto val : pi) {
                        sum += val;
                    }
                    REQUIRE(sum == 5);
                    break;
                }
                case TestComplex::Test::optional_string_s: {
                    REQUIRE(item.get_string() == "optionalstring");
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(number_of_u == 5);
    }

    SECTION("skip everything") {
        const std::string buffer = load_data("complex/data-all");

        protozero::pbf_message<TestComplex::Test> item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case TestComplex::Test::required_fixed32_f:
                case TestComplex::Test::optional_int64_i:
                case TestComplex::Test::optional_int64_j:
                case TestComplex::Test::repeated_uint32_u:
                case TestComplex::Test::required_Sub_submessage:
                case TestComplex::Test::packed_sint32_d:
                case TestComplex::Test::optional_string_s:
                    item.skip();
                    break;
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

}

TEST_CASE("write complex data using pbf_writer") {

    SECTION("minimal") {
        std::string buffer;
        protozero::pbf_writer pw(buffer);
        pw.add_fixed32(1, 12345678);

        std::string submessage;
        protozero::pbf_writer pws(submessage);
        pws.add_string(1, "foobar");

        pw.add_message(5, submessage);

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

    SECTION("some") {
        std::string buffer;
        protozero::pbf_writer pw2(buffer);
        pw2.add_fixed32(1, 12345678);

        protozero::pbf_writer pw;
        using std::swap;
        swap(pw, pw2);

        std::string submessage;
        protozero::pbf_writer pws(submessage);
        pws.add_string(1, "foobar");

        pw.add_uint32(4, 22);
        pw.add_uint32(4, 44);
        pw.add_int64(2, -9876543);
        pw.add_message(5, submessage);

        protozero::pbf_reader item(buffer);

        uint32_t sum_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 4: {
                    sum_of_u += item.get_uint32();
                    break;
                }
                case 5: {
                    auto view = item.get_view();
                    protozero::pbf_reader subitem{view};
                    REQUIRE(subitem.next());
                    REQUIRE(std::string(subitem.get_view()) == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(sum_of_u == 66);
    }

    SECTION("all") {
        std::string buffer;
        protozero::pbf_writer pw(buffer);
        pw.add_fixed32(1, 12345678);

        std::string submessage;
        protozero::pbf_writer pws(submessage);
        pws.add_string(1, "foobar");
        pw.add_message(5, submessage);

        pw.add_uint32(4, 22);
        pw.add_uint32(4, 44);
        pw.add_int64(2, -9876543);
        pw.add_uint32(4, 44);
        pw.add_uint32(4, 66);
        pw.add_uint32(4, 66);

        int32_t d[] = { -17, 22 };
        pw.add_packed_sint32(7, std::begin(d), std::end(d));

        pw.add_int64(3, 555555555);

        protozero::pbf_reader item(buffer);

        int number_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 3: {
                    REQUIRE(item.get_int64() == 555555555LL);
                    break;
                }
                case 4: {
                    item.skip();
                    ++number_of_u;
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case 7: {
                    const auto pi = item.get_packed_sint32();
                    int32_t sum = 0;
                    for (auto val : pi) {
                        sum += val;
                    }
                    REQUIRE(sum == 5);
                    break;
                }
                case 8: {
                    REQUIRE(item.get_string() == "optionalstring");
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(number_of_u == 5);
    }
}

TEST_CASE("write complex data using pbf_builder") {

    SECTION("minimal") {
        std::string buffer;
        protozero::pbf_builder<TestComplex::Test> pw(buffer);
        pw.add_fixed32(TestComplex::Test::required_fixed32_f, 12345678);

        std::string submessage;
        protozero::pbf_builder<TestComplex::Sub> pws(submessage);
        pws.add_string(TestComplex::Sub::required_string_s, "foobar");

        pw.add_message(TestComplex::Test::required_Sub_submessage, submessage);

        protozero::pbf_reader item(buffer);

        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
    }

    SECTION("some") {
        std::string buffer;
        protozero::pbf_builder<TestComplex::Test> pw2(buffer);
        pw2.add_fixed32(TestComplex::Test::required_fixed32_f, 12345678);

        std::string dummy_buffer;
        protozero::pbf_builder<TestComplex::Test> pw(dummy_buffer);
        using std::swap;
        swap(pw, pw2);

        std::string submessage;
        protozero::pbf_builder<TestComplex::Sub> pws(submessage);
        pws.add_string(TestComplex::Sub::required_string_s, "foobar");

        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 22);
        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 44);
        pw.add_int64(TestComplex::Test::optional_int64_i, -9876543);
        pw.add_message(TestComplex::Test::required_Sub_submessage, submessage);

        protozero::pbf_reader item(buffer);

        uint32_t sum_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 4: {
                    sum_of_u += item.get_uint32();
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(sum_of_u == 66);
    }

    SECTION("all") {
        std::string buffer;
        protozero::pbf_builder<TestComplex::Test> pw(buffer);
        pw.add_fixed32(TestComplex::Test::required_fixed32_f, 12345678);

        std::string submessage;
        protozero::pbf_builder<TestComplex::Sub> pws(submessage);
        pws.add_string(TestComplex::Sub::required_string_s, "foobar");
        pw.add_message(TestComplex::Test::required_Sub_submessage, submessage);

        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 22);
        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 44);
        pw.add_int64(TestComplex::Test::optional_int64_i, -9876543);
        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 44);
        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 66);
        pw.add_uint32(TestComplex::Test::repeated_uint32_u, 66);

        int32_t d[] = { -17, 22 };
        pw.add_packed_sint32(TestComplex::Test::packed_sint32_d, std::begin(d), std::end(d));

        pw.add_int64(TestComplex::Test::optional_int64_j, 555555555);

        protozero::pbf_reader item(buffer);

        int number_of_u = 0;
        while (item.next()) {
            switch (item.tag()) {
                case 1: {
                    REQUIRE(item.get_fixed32() == 12345678L);
                    break;
                }
                case 2: {
                    REQUIRE(true);
                    item.skip();
                    break;
                }
                case 3: {
                    REQUIRE(item.get_int64() == 555555555LL);
                    break;
                }
                case 4: {
                    item.skip();
                    ++number_of_u;
                    break;
                }
                case 5: {
                    protozero::pbf_reader subitem = item.get_message();
                    REQUIRE(subitem.next());
                    REQUIRE(subitem.get_string() == "foobar");
                    REQUIRE(!subitem.next());
                    break;
                }
                case 7: {
                    const auto pi = item.get_packed_sint32();
                    int32_t sum = 0;
                    for (auto val : pi) {
                        sum += val;
                    }
                    REQUIRE(sum == 5);
                    break;
                }
                case 8: {
                    REQUIRE(item.get_string() == "optionalstring");
                    break;
                }
                default: {
                    REQUIRE(false); // should not be here
                    break;
                }
            }
        }
        REQUIRE(number_of_u == 5);
    }
}

static void check_message(const std::string& buffer) {
    protozero::pbf_reader item(buffer);

    while (item.next()) {
        switch (item.tag()) {
            case 1: {
                REQUIRE(item.get_fixed32() == 42L);
                break;
            }
            case 5: {
                protozero::pbf_reader subitem = item.get_message();
                REQUIRE(subitem.next());
                REQUIRE(subitem.get_string() == "foobar");
                REQUIRE(!subitem.next());
                break;
            }
            default: {
                REQUIRE(false); // should not be here
                break;
            }
        }
    }
}

TEST_CASE("write complex with subwriter using pbf_writer") {
    std::string buffer_test;
    protozero::pbf_writer pbf_test(buffer_test);
    pbf_test.add_fixed32(1, 42L);

    SECTION("message in message") {
        protozero::pbf_writer pbf_submessage(pbf_test, 5);
        pbf_submessage.add_string(1, "foobar");
    }

    check_message(buffer_test);
}

TEST_CASE("write complex with subwriter using pbf_builder") {
    std::string buffer_test;
    protozero::pbf_builder<TestComplex::Test> pbf_test(buffer_test);
    pbf_test.add_fixed32(TestComplex::Test::required_fixed32_f, 42L);

    SECTION("message in message") {
        protozero::pbf_builder<TestComplex::Sub> pbf_submessage(pbf_test, TestComplex::Test::required_Sub_submessage);
        pbf_submessage.add_string(TestComplex::Sub::required_string_s, "foobar");
    }

    check_message(buffer_test);
}

