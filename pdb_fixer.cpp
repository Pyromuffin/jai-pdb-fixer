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

    // copy the file into the buffer:
    auto result = fread(buffer, 1, lSize, file);
    
    std::string replacement;
    replacement.push_back('a');
    replacement.push_back('<');

    int count = 0;
    for (int i = 0; i < lSize - 2; i++)
    {
        if (buffer[i] == '[' && buffer[i + 1] == ']' && buffer[i + 2] == ' ')
        {
            auto startIndex = i;
            auto endIndex = i + 3;
            replacement.resize(2);
            while (buffer[endIndex] != '\0')
            {
                replacement.push_back(buffer[endIndex]);
                endIndex++;
            }
            replacement.push_back('>');

            fseek(file, i, SEEK_SET);
            fputs(replacement.c_str(), file);
            i = endIndex;
            count++;
        }
    }

    fclose(file);
    free(buffer);

    printf("pdb fixed up %d arrays.\n", count);
    return 0;
}