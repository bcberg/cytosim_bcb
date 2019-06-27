// Cytosim was created by Francois Nedelec. Copyright 2007-2017 EMBL.
#include <vector>
#include <string>

/// Functions to handle UNIX-like directory paths
namespace FilePath
{
    /// true if 'path' is an existing readible file
    bool        is_file(const char path[]);
    
    /// true if 'path' is an existing readible file
    bool        is_file(std::string const& str);
    
    /// true if 'path' is an existing directory
    bool        is_dir(const char path[]);
    
    /// true if 'path' is an existing directory
    bool        is_dir(std::string const& str);

    /// return path to current working directory
    std::string get_cwd();
    
    /// change current working directory
    int         change_dir(std::string const& path);

    /// create new directory
    int         make_dir(const char path[]);
    
    /// list content of directory
    std::vector<std::string> list_dir(std::string const& path);
    
    /// list files in a directory that have a name finishing with 'ext'
    std::vector<std::string> list_dir(std::string const& path, std::string const& ext);
   
    /// extract the directory part from the given path
    std::string dir_part(std::string const& path);

    /// extract the file part from the given path
    std::string file_part(std::string const& path);

    /// complete the file name using the given directory
    std::string full_name(std::string const& dir, std::string const& file);

}


