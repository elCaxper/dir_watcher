#pragma once

#include <iostream>
#include <vector>

#include <filesystem>
#include <functional>   // std::function, std::negate

#include <thread>
#include <chrono>

namespace fs = std::filesystem;

enum class FileStatus {created, modified, erased};

class Watcher{

    std::vector<fs::path> mFsPath;

    std::vector<std::string> mSv;
public:
    Watcher();

    void add_path_monitor(const fs::path & path);

    void print();

    std::vector<std::string> getDirectoryFiles(const std::string & dir, const std::vector<std::string> & extensions);

    std::vector<std::string> getDirectoryFiles(const std::string & dir);

    std::pair<fs::path,std::string> getPathFilterPair( const fs::path &path );

    std::pair<fs::path,std::string> visitWildCardPath( const fs::path &path, 
        const std::function<bool(const fs::path&)> &visitor );


    std::vector<std::string> get_files( const fs::path &path);

    void start(const std::function<void (std::string, FileStatus)> &action);

private:
     std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    std::string path_to_watch;

    bool running_ = true;
    std::chrono::duration<int, std::milli> delay;

     bool contains(const std::string &key) {
         auto el = paths_.find(key);
         return el != paths_.end();
    }
};
