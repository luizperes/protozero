
#include <test.hpp>

TEST_CASE("read repeated packed enum field") {

    SECTION("empty") {
        const std::string buffer = load_data("repeated_packed_enum/data-empty");

        protozero::pbf_reader item(buffer);

        REQUIRE(!item.next());
    }

    SECTION("one") {
        const std::string buffer = load_data("repeated_packed_enum/data-one");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_range = item.get_packed_enum();
        REQUIRE(!item.next());

        REQUIRE(it_range.begin() != it_range.end());
        REQUIRE(*it_range.begin() == 0 /* BLACK */);
        REQUIRE(std::next(it_range.begin()) == it_range.end());
    }

    SECTION("many") {
        const std::string buffer = load_data("repeated_packed_enum/data-many");

        protozero::pbf_reader item(buffer);

        REQUIRE(item.next());
        auto it_range = item.get_packed_enum();
        REQUIRE(!item.next());

        auto it = it_range.begin();
        REQUIRE(it != it_range.end());
        REQUIRE(*it++ == 0 /* BLACK */);
        REQUIRE(*it++ == 3 /* BLUE */);
        REQUIRE(*it++ == 2 /* GREEN */);
        REQUIRE(it == it_range.end());
    }

    SECTION("end_of_buffer") {
        const std::string buffer = load_data("repeated_packed_enum/data-many");

        for (std::string::size_type i = 1; i < buffer.size(); ++i) {
            protozero::pbf_reader item(buffer.data(), i);
            REQUIRE(item.next());
            REQUIRE_THROWS_AS(item.get_packed_enum(), protozero::end_of_buffer_exception);
        }
    }

}

TEST_CASE("write repeated packed enum field") {

    std::string buffer;
    protozero::pbf_writer pw(buffer);

    SECTION("empty") {
        int32_t data[] = { 0 /* BLACK */ };
        pw.add_packed_enum(1, std::begin(data), std::begin(data) /* !!!! */);

        REQUIRE(buffer == load_data("repeated_packed_enum/data-empty"));
    }

    SECTION("one") {
        int32_t data[] = { 0 /* BLACK */ };
        pw.add_packed_enum(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_enum/data-one"));
    }

    SECTION("many") {
        int32_t data[] = { 0 /* BLACK */, 3 /* BLUE */, 2 /* GREEN */ };
        pw.add_packed_enum(1, std::begin(data), std::end(data));

        REQUIRE(buffer == load_data("repeated_packed_enum/data-many"));
    }

}

