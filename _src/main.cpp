// #include "__preprocessor__.h"
#include "message.pb.h"

// #ifdef BUILD_EXECUTABLE
// int main(int argc, char* argv[])
// {
//     srand(time(NULL));
//     // CORE::clear_terminal(); // tests will NOT be VISIBLE with this line
//     time_stamp("It just works");

//     CORE::str::split_string("Hello World!", ' ');
//     var(CORE::str::to_lower_case("Hello, World!"));

//     show_sizeof(test);
//     show_sizeof_many(test, 100);

//     int num = 123456789;
//     double num2 = 1234567.89123;

//     cout << CORE::format_number(num) << endl;
//     cout << CORE::format_number(num2) << endl;

//     return 0;
// }
// #endif

#ifdef BUILD_EXECUTABLE
int main()
{
    // Inicjalizacja protobuf
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // Tworzenie i ustawianie danych
    tutorial::Person person;
    person.set_name("Jan Kowalski");
    person.set_id(123);
    person.set_email("jan.kowalski@example.com");

    // Serializacja do pliku
    std::ofstream output("person.bin", std::ios::binary);
    if (!person.SerializeToOstream(&output))
    {
        std::cerr << "Błąd podczas zapisu danych." << std::endl;
        return -1;
    }
    output.close();

    // Deserializacja z pliku
    tutorial::Person person_read;
    std::ifstream input("person.bin", std::ios::binary);
    if (!person_read.ParseFromIstream(&input))
    {
        std::cerr << "Błąd podczas odczytu danych." << std::endl;
        return -1;
    }
    input.close();

    // Wyświetlenie odczytanych danych
    std::cout << "Imię: " << person_read.name() << std::endl;
    std::cout << "ID: " << person_read.id() << std::endl;
    std::cout << "Email: " << person_read.email() << std::endl;

    // Zwolnienie zasobów
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
#endif