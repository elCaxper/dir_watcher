#include <iostream>
#include "watcher.h"

#include <filesystem>

//#include <vector>
#include <string>

#include <functional>


bool visitor(const fs::path& path)
{
    std::cout << "visitor: " <<path << '\n';

    return true;
}

int main() {

    std::vector<int> v;
    std::string s;
    s.find(2);
    v.push_back(1);
    Watcher w;

    auto path1 = std::filesystem::path("./");
    
    /*w.print();
    w.add_path_monitor(path1);

    w.getDirectoryFiles(path1);*/


    /*auto path2 = std::filesystem::path("./CMakeFiles/3.12.0/*.cmake");
    w.getPathFilterPair(path2);

    std::function<bool(const fs::path&)> f_display = visitor;
    w.visitWildCardPath(path2,visitor);*/   


    w.start([] (std::string path_to_watch, FileStatus status) -> void {
        std::cout << "File Something: " << path_to_watch << '\n';

         // Process only regular files, all other file types are ignored
         if(!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileStatus::erased) {
             return;
         }
 
         switch(status) {
             case FileStatus::created:
                 std::cout << "File created: " << path_to_watch << '\n';
                 break;
             case FileStatus::modified:
                 std::cout << "File modified: " << path_to_watch << '\n';
                 break;
             case FileStatus::erased:
                 std::cout << "File erased: " << path_to_watch << '\n';
                 break;
             default:
                 std::cout << "Error! Unknown file status.\n";
         }
     });

    return 0;
}
