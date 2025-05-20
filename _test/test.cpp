// clang-format off
#include <gtest/gtest.h> // must be first
// clang-format on
#include "__preprocessor__.h"
#include "message.pb.h"

// 3) TEST_P + INSTANTIATE_TEST_CASE_P – testy parametryzowane
typedef std::tuple<string, int, string> input_data;
class parsing_test : public ::testing::TestWithParam<input_data>
{
};

// clang-format off
INSTANTIATE_TEST_CASE_P(Protobuff, parsing_test, 
::testing::Values(
                    std::make_tuple("Jan Kowalski", 123, "email")
                    , std::make_tuple("Konrad Trzaskowski", 321, "secret email")
                    , std::make_tuple("Anna Nowak-Kowalska", 456, "anna.nowak@email.com")
                    , std::make_tuple("Łukasz Żółć", 789, "lukasz.zolc@domain.pl")
                    , std::make_tuple("John Doe", 999, "john.doe@company.com")
                    , std::make_tuple("", 0, "")  // Edge case with empty strings
                    , std::make_tuple("Very Long Name That Tests Buffer Capacity", 1000000, "very.long.email.address@some.domain.com")
                    , std::make_tuple("Special Ch@r Name!", 42, "special.chars+123@domain.co.uk")
));
// clang-format on

TEST_P(parsing_test, ARRAY)
{
    auto [name, id, email] = GetParam();

    // Inicjalizacja protobuf
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::Person person;
    tutorial::Person person_read;
    vector<u8> buffer;

    person.set_name(name);
    person.set_id(id);
    person.set_email(email);

    // Obliczanie rozmiaru wiadomości
    int size = person.ByteSizeLong();
    buffer.resize(size);

    // Serializacja do bufora
    if (!person.SerializeToArray(buffer.data(), size))
    {
        std::cerr << "Błąd podczas serializacji wiadomości." << std::endl;
        FAIL();
    }

    if (!person_read.ParseFromArray(buffer.data(), size))
    {
        std::cerr << "Błąd podczas deserializacji wiadomości." << std::endl;
        FAIL();
    }

    EXPECT_EQ(person.name(), person_read.name());
    EXPECT_EQ(person.id(), person_read.id());
    EXPECT_EQ(person.email(), person_read.email());
}

TEST_P(parsing_test, FILE)
{
    auto [name, id, email] = GetParam();

    // Inicjalizacja protobuf
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tutorial::Person person;
    tutorial::Person person_read;

    person.set_name(name);
    person.set_id(id);
    person.set_email(email);

    // Serializacja do pliku
    std::ofstream output("person.bin", std::ios::binary);
    if (!person.SerializeToOstream(&output))
    {
        std::cerr << "Błąd podczas zapisu danych." << std::endl;
        FAIL();
    }
    output.close();

    // Deserializacja z pliku
    std::ifstream input("person.bin", std::ios::binary);
    if (!person_read.ParseFromIstream(&input))
    {
        std::cerr << "Błąd podczas odczytu danych." << std::endl;
        FAIL();
    }
    input.close();

    EXPECT_EQ(person.name(), person_read.name());
    EXPECT_EQ(person.id(), person_read.id());
    EXPECT_EQ(person.email(), person_read.email());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
