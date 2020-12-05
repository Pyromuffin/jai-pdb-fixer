#include <iostream>

int main(int argc, char** args)
{
    if (argc <= 1)
        return 0;

    auto path = args[1];

    FILE* file;
    auto fileResult = fopen_s(&file, path, "rb+");

    if (fileResult != 0)
        return fileResult;

    fseek(file, 0, SEEK_END);
    auto lSize = ftell(file);
    rewind(file);

    auto buffer = (char*)malloc(sizeof(char) * lSize);
    if (!buffer)
        return -1;

    // copy the file into the buffer:
    auto result = fread(buffer, 1, lSize, file);
    
    std::string replacement;


    int count = 0;
    for (int i = 0; i < lSize - 5; i++)
    {
        if (buffer[i] == '[' && buffer[i + 1] == ']' && buffer[i + 2] == ' ')
        {
            auto startIndex = i;
            auto endIndex = i + 3;
            replacement.clear();

            replacement.push_back('a');
            replacement.push_back('<');
            while (buffer[endIndex] != '\0')
            {
                replacement.push_back(buffer[endIndex]);
                endIndex++;
            }
            replacement.push_back('>');

            memcpy(&buffer[i], replacement.c_str(), replacement.size());

            count++;
        }

        else if (buffer[i] == '[' && buffer[i + 1] == '.' && buffer[i + 2] == '.' && buffer[i + 3] == ']' && buffer[i + 4] == ' ')
        {
            auto startIndex = i;
            auto endIndex = i + 5;
            replacement.clear();
            replacement.push_back('v');
            replacement.push_back('e');
            replacement.push_back('c');
            replacement.push_back('<');
            while (buffer[endIndex] != '\0')
            {
                replacement.push_back(buffer[endIndex]);
                endIndex++;
            }
            replacement.push_back('>');

            memcpy(&buffer[i], replacement.c_str(), replacement.size());

            count++;
        }

    }

    if (count == 0)
        goto done;

    rewind(file);
    fwrite(buffer, 1, lSize, file);

    done:
    fclose(file);
    free(buffer);

    printf("pdb fixed up %d arrays.\n", count);
    return 0;
}