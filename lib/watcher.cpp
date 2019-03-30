#include <watcher.h>
#include <algorithm>    // std::find

#include <map>
#include <string>

Watcher::Watcher() {
    delay = std::chrono::milliseconds(5000);
    path_to_watch = std::filesystem::path("./*.cmake");
    /*for(auto &file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
             paths_[file.path()] = std::filesystem::last_write_time(file);
    }*/

    for(auto &file : get_files(path_to_watch)) {
            std::cout << "file: " << file << '\n';
            paths_[file] = std::filesystem::last_write_time(file);
    }
    
}

void Watcher::print() {
    std::cout << "Actual Path:" << '\n';
    std::cout << fs::current_path() << '\n';
}

void Watcher::add_path_monitor(const fs::path &path) {
    mFsPath.push_back(path);
    mFsPath.pop_back();


}

std::vector<std::string> Watcher::getDirectoryFiles(const std::string & dir, const std::vector<std::string> & extensions)
{
        std::vector<std::string> files;
    for(auto & p: fs::recursive_directory_iterator(dir))
    {
        if (fs::is_regular_file(p))
        {
            std::cout << "File: " << p.path().string() << '\n';
                if (extensions.empty() || std::find(extensions.begin(), extensions.end(), p.path().extension().string()) != extensions.end())
            {
                files.push_back(p.path().string());
            }
        }
    }
    return files;
}

std::vector<std::string> Watcher::getDirectoryFiles(const std::string & dir)
{
        std::vector<std::string> files;
    for(auto & p: fs::recursive_directory_iterator(dir))
    {
        if (fs::is_regular_file(p))
        {
            std::cout << "File: " << p.path().string() << '\n';
        }
    }
    return files;
}


std::pair<fs::path,std::string> Watcher::getPathFilterPair( const fs::path &path )
{
        // extract wildcard and parent path
        std::string key     = path.string();
        fs::path p      = path;
        size_t wildCardPos  = key.find( "*" );
        std::string filter;
        if( wildCardPos != std::string::npos ){
            filter  = path.filename().string();
            p       = path.parent_path();
            /*std::cout << "filter: " <<  filter << '\n';
            std::cout << "parent_path: " <<  p << '\n';
            std::cout << "parent_path absolute: " <<  fs::absolute(p) << '\n';*/

        }
        

        // throw an exception if the file doesn't exist
        if( filter.empty() && !fs::exists( p ) ){
            std::cout << "File ERROR" << '\n';
            //throw WatchedFileSystemExc( path );
        }
        
        return std::make_pair( p, filter );
            
}


std::pair<fs::path,std::string> Watcher::visitWildCardPath( const fs::path &path, 
const std::function<bool(const fs::path&)> &visitor ){

        std::pair<fs::path, std::string> pathFilter = getPathFilterPair( path );
        if( !pathFilter.second.empty() ){
            std::string full    = ( pathFilter.first / pathFilter.second ).string();
            size_t wildcardPos  = full.find( "*" );
            std::string before  = full.substr( 0, wildcardPos );
            std::string after   = full.substr( wildcardPos + 1 );
            fs::recursive_directory_iterator end;
            for( fs::recursive_directory_iterator it( pathFilter.first ); it != end; ++it ){
                std::string current = it->path().string();
                size_t beforePos    = current.find( before );
                size_t afterPos     = current.find( after );
                if( ( beforePos != std::string::npos || before.empty() )
                   && ( afterPos != std::string::npos || after.empty() ) ) {
                    //std::cout << "it->path(): " << it->path() << '\n';
                    if( visitor( it->path() ) ){
                        //break;
                    }
                }
            }
        }
        return pathFilter;
}



std::vector<std::string> Watcher::get_files( const fs::path &path){
    std::cout << "get_files" << std::endl;

    std::vector<std::string> vs;
        std::pair<fs::path, std::string> pathFilter = getPathFilterPair( path );
        if( !pathFilter.second.empty() ){
            std::string full    = ( pathFilter.first / pathFilter.second ).string();
            size_t wildcardPos  = full.find( "*" );
            std::string before  = full.substr( 0, wildcardPos );
            std::string after   = full.substr( wildcardPos + 1 );
            fs::recursive_directory_iterator end;
            for( fs::recursive_directory_iterator it( pathFilter.first ); it != end; ++it ){
                std::string current = it->path().string();
                size_t beforePos    = current.find( before );
                size_t afterPos     = current.find( after );
                if( ( beforePos != std::string::npos || before.empty() )
                   && ( afterPos != std::string::npos || after.empty() ) ) {
                    //std::cout << "it->path(): " << it->path() << '\n';
                    vs.push_back(it->path().string());
                    /*if( visitor( it->path() ) ){
                        //break;
                    }*/
                }
            }
        }
        return vs;
        //return pathFilter;
}
 
//TODO: there is a seg fault when we remove a file.
void Watcher::start(const std::function<void (std::string, FileStatus)> &action) {
         while(running_) {
             // Wait for "delay" milliseconds
             std::this_thread::sleep_for(delay);
 
             // Check if one of the old files was erased
             for(auto &el : paths_) {
                //std::cout << "el1: " << el.first << '\n';
                //std::cout << "el2: " << el.second << '\n';

                 if(!std::filesystem::exists(el.first)) {
                     std::cout << "el1: " << el.first << '\n';
                     action(el.first, FileStatus::erased);
                     paths_.erase(el.first);
                 }
             }
            //std::cout << "inside: " << '\n';

             // Check if a file was created or modified
             //auto pathg =  fs::absolute(fs::path(path_to_watch)).parent_path() ;
             //std::cout << "fs::path(path_to_watch): " << fs::path(path_to_watch) <<'\n';
             //std::cout << "fs::absolute(path_to_watch): " << fs::absolute(fs::path(path_to_watch)) <<'\n';

            //std::cout << "pathg: " << pathg <<'\n';

             
             for(auto &file : get_files(fs::path(path_to_watch))) {
                 auto file_p = fs::path(file);
                 //std::cout << "checking file: " << file <<'\n';
                 auto current_file_last_write_time = std::filesystem::last_write_time(file);
                 // File creation
                 // TODO: ggpr we have to check if the file is not in list and the file use the pattern
                 if(!contains(file_p)) {
                     paths_[file_p] = current_file_last_write_time;
                     action(file_p, FileStatus::created);
                 // File modification
                 } else {
                     if(paths_[file_p] != current_file_last_write_time) {
                         paths_[file_p] = current_file_last_write_time;
                         action(file_p, FileStatus::modified);
                     }
                 }
             }
         }
}