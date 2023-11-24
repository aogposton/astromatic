#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <json.h>
#include <list>
#include <map>
#include <stdlib.h> 

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

namespace fs = std::filesystem;

std::list<std::string> readDirectory(std::string location){
    std::list<std::string> contents{};
    
    for (const auto & entry : fs::directory_iterator(location)){
        std::string path = entry.path();
        if(path.find("config.ts")==std::string::npos){
            contents.push_back(entry.path());
        }
    } 
    return contents;
}

void clear()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

void spacer(int count){
    for(int i; i<count; i++){
        std::cout << std::endl;
    }
}

std::map<std::string, std::string> getMetadata(std::ifstream &myfile){
    bool startingLine, endingLine = false;
    int dataTypeDelimeterPos;
    std::map<std::string, std::string> contentMap;
    std::string defaultValue;
    std::string key;
    if(myfile.is_open()){
        while(myfile){
            for (std::string line; std::getline(myfile, line); ) 
            {
                if(!startingLine&&line.find("---")!= std::string::npos){
                    startingLine = true;
                    continue;
                }

                if(startingLine&&line.find("---")== std::string::npos){
                    // std::cout << line << std::endl;
                    //start extraction
                    dataTypeDelimeterPos=line.find(':');
                    defaultValue = line.substr(dataTypeDelimeterPos+1);    
                    key = line.substr(0,dataTypeDelimeterPos);
                    contentMap.insert({key,defaultValue});
                }
                
                if(startingLine&&line.find("---") != std::string::npos){
                    endingLine = true;
                }

                if(startingLine&&endingLine){
                    break;
                }
            }
        }
    }
    return contentMap;
 
}

int main() {
    std::ifstream ifs("astromatic.json");
    Json::Reader reader;
    Json::Value obj;
    std::string location = "./src/content";
    std::string contentTemplate;
    std::list<std::string> contentFolders = readDirectory(location);
    std::string folder;
    std::string selectedfolder;
    std::ifstream myfile;
    std::string newFileContent;
    std::ofstream newFile;
    
    clear();

    if (ifs.good())
    {
        reader.parse(ifs, obj); // reader can also read strings
        location = obj["contentFolder"].asString();
    }
    
    
    for (const std::string i : contentFolders){
        if (i.find(location) != std::string::npos){
            folder = i.substr(location.size()+1);
        }
        std::cout << folder << std::endl;
    }

    std::cout<< "Which kind of file?" << std::endl;
    std::cin >> contentTemplate;
    std::cin.ignore();
    

    for (const auto i : contentFolders){
        if(i.find(location+"/"+contentTemplate)!= std::string::npos){
            selectedfolder = i;
        }
    }

    myfile.open(readDirectory(selectedfolder).front());
    clear();

    std::map<std::string, std::string> metadata = getMetadata(myfile);

    newFileContent.append("---");
    newFileContent.append("\n");
    std::string title;

    for (const auto& x : metadata) {
        std::string input;
        std::cout << x.first +" (enter to skip)"+ ":";
        std::getline(std::cin, input);

        if(input==""){
            std::cout << "skipped data";
            continue;
        }
        
        if(x.first == "title"){
            title = input;
        }

        newFileContent.append(x.first+":");
        newFileContent.append(input);
        newFileContent.append("\n");
    }
    
    newFileContent.append("---");
    
    clear();
    spacer(3);
    
    std::cout << newFileContent;

    newFile.open(location+"/"+folder+"/"+title+".md");
    newFile << newFileContent;
    newFile.close();

}
