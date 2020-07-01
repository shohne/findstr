#include <sys/stat.h>
#include <iostream>
#include <string>
#include <filesystem>

int howManyLinesBetween(std::string &str, int begin, int end)
{
    int result = 0;
    for (int i = begin; i < end; ++i)
    {
        if (str[i] == '\n')
            result++;
    }
    return result;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "Usage: findstr patter <<directory>>" << std::endl;
        return 1;
    }

    auto start = std::chrono::system_clock::now();
    int i = 0;
    long bytesRead = 0;

    std::string pattern = std::string(argv[1]);

    std::string basePath = ".";
    if (argc >= 3)
    {
        basePath = std::string(argv[2]);
    }

    for (auto &entry : std::filesystem::recursive_directory_iterator(basePath))
    {
        if (std::filesystem::is_regular_file(entry.path()))
        {
            struct stat info;
            const char *filename = entry.path().string().c_str();
            stat(filename, &info);
            char *content = (char *)malloc(info.st_size);
            FILE *fp = fopen(filename, "rb");
            size_t blocks_read = fread(content, info.st_size, 1, fp);
            fclose(fp);
//            std::string s(std::move(content));
            std::string s(content);
            free(content);

            int line = 1;
            int last_position = 0;
            std::size_t found = s.find(pattern, last_position);
            while (found != std::string::npos)
            {
                line += howManyLinesBetween(s, last_position, found);
                last_position = found + 1;
                std::cout << entry.path().string() << ": " << line << std::endl;
                found = s.find(pattern, found + 1);
            }

            bytesRead += (unsigned long)info.st_size;
            i++;
        }
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << i << " files" << " (" << std::to_string(elapsed_seconds.count()) << "s)"  << std::endl;

    return 0;
}
