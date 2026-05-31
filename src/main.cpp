#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iomanip>

#include "templates/TemplateEngine.h"
#include "database/Database.h"
#include "models/Person.h"


std::string formatTime(std::time_t t) {
	std::tm *tm = std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

int main(int argc, const char * argv[]) {

	std::filesystem::path appDir;

	#ifdef _WIN32
		appDir = std::getenv("USERPROFILE");
	#else
		appDir = std::getenv("HOME");
	#endif

	appDir /= "Documents/boap-sem-ii-cw";

	std::filesystem::path dbPath = appDir / "database" / "persons.db";

	Database db(dbPath.string());
	db.init();

	int id;
	std::cout << "Enter person ID: ";
	std::cin >> id;

	Person person = db.getPersonById(id);

	std::unordered_map<std::string, std::string> values = {
		{"FIRST_NAME", person.firstName},
		{"LAST_NAME", person.lastName},
		{"PATRONYMIC", person.patronymic},

		{"AGE", std::to_string(person.age)},
		{"GENDER", person.gender},

		{"CITY", person.city},
		{"ADDRESS", person.address},

		{"EMAIL", person.email},
		{"PHONE", person.phone},

		{"POSITION", person.position},

		{"CREATED_AT", formatTime(person.createdAt)},
		{"UPDATED_AT", formatTime(person.updatedAt)}
	};

    std::ifstream templateFile("src/templates/template.txt");

    if (!templateFile.is_open()) {
        std::cerr << "Failed to open template file\n";
        return EXIT_FAILURE;
    }

    std::stringstream buffer;
    buffer << templateFile.rdbuf();

    std::string templateText = buffer.str();
    std::string result = TemplateEngine::replacePlaceholders(templateText, values);

	std::filesystem::path outputDir = appDir / "output";
	std::filesystem::create_directories(outputDir);

	std::string fileName =
		std::to_string(person.id) + "_" +
		person.lastName + "_" +
		person.firstName + "_" +
		person.patronymic;

	std::replace(fileName.begin(), fileName.end(), ' ', '_');
	fileName += ".txt";

	std::filesystem::path outputFilePath = outputDir / fileName;

	std::ofstream outputFile(outputFilePath);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to create output file\n";
        return EXIT_FAILURE;
    }

    outputFile << result;
    outputFile.close();

    std::cout << "Generated document:\n\n";
    std::cout << result;
    std::cout << "\nSaved to " << outputFilePath << "\n";

    return EXIT_SUCCESS;
}
