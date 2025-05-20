#include "__preprocessor__.h"
#include "message.pb.h"

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

    tutorial::Person person_read;

    if (1)
    {
        // Obliczanie rozmiaru wiadomości
        int size = person.ByteSizeLong();
        vector<u8> buffer;
        buffer.resize(size);

        // Serializacja do bufora
        if (!person.SerializeToArray(buffer.data(), size))
        {
            std::cerr << "Błąd podczas serializacji wiadomości." << std::endl;
            return -1;
        }

        if (!person_read.ParseFromArray(buffer.data(), size))
        {
            std::cerr << "Błąd podczas deserializacji wiadomości." << std::endl;
            return -1;
        }
    }
    else
    {
        // Serializacja do pliku
        std::ofstream output("person.bin", std::ios::binary);
        if (!person.SerializeToOstream(&output))
        {
            std::cerr << "Błąd podczas zapisu danych." << std::endl;
            return -1;
        }
        output.close();

        // Deserializacja z pliku
        std::ifstream input("person.bin", std::ios::binary);
        if (!person_read.ParseFromIstream(&input))
        {
            std::cerr << "Błąd podczas odczytu danych." << std::endl;
            return -1;
        }
        input.close();
    }

    // Wyświetlenie odczytanych danych
    std::cout << "Imię: " << person_read.name() << std::endl;
    std::cout << "ID: " << person_read.id() << std::endl;
    std::cout << "Email: " << person_read.email() << std::endl;

    // Zwolnienie zasobów
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
#endif