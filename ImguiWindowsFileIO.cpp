
#include "ImguiWindowsFileIO.hpp"

#include <algorithm>
#include <cstdlib>
#include <sstream>

#if defined(_WINDOWS)
    #include <windows.h>
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#if defined(ICON_FA_CARET_DOWN)
#define CARET_DOWN ICON_FA_CARET_DOWN
#else
#define CARET_DOWN "v"
#endif

using namespace std;
using namespace ImGui;
 

vector<string> stringSplit(const string &s, char delim)
{
vector<string> elems;
string item; 

// use stdlib to tokenize the string
stringstream ss(s);
	while (getline(ss, item, delim))
		if(!item.empty())
		{
			elems.push_back(item);
		}

	return elems;
}

string MiniPath::filePath() const
{
    return path + name;
}

void MiniPath::fromString (const string& file_path, char delim)
{
    int last_delim_pos = file_path.find_last_of (delim);
    name = file_path.substr (last_delim_pos+1);
    path = file_path.substr (0, last_delim_pos+1);
}

MiniPath::MiniPath()
{}

MiniPath::MiniPath( const string& some_path )
{
	string s = some_path;
	if (MiniPath::isAbsoluteFilePath (s))
	{
		if (s.find ("/") != string::npos) // linux style
			fromString (s ,'/');
		else if (s.find ("\\") != string::npos) // windows style
			fromString (s ,'\\');
	}
	else
	{
		string current = MiniPath::getCurrentDir();

		if (current.find ("/") != string::npos)
		{
			std::replace( s.begin(), s.end(), '\\', '/');
			fromString (current + "/" + s ,'/');
		}
		else if (current.find ("\\") != string::npos)
		{
			std::replace( s.begin(), s.end(), '/', '\\');
			fromString (current + "\\" + s ,'\\');
		}
		else
			fromNameInCurrentDir( s );
	}
}

string MiniPath::getDelim() const
{
	if (path.find ("/") != string::npos) // linux style
			return "/";
	else if (path.find ("\\") != string::npos) // windows style
			return "\\";
	else
	{
		string current = MiniPath::getCurrentDir();
		if (current.find ("/") != string::npos) // linux style
			return "/";
		else if (current.find ("\\") != string::npos) // windows style
			return "\\";
		else
			return "/";
	}
}

string MiniPath::prefix() const
{
    return name.substr (0, name.find_last_of ('.'));
}

string MiniPath::extension() const
{
	if (name.find (".") == string::npos)
		return "";

    return name.substr (name.find_last_of ('.')+1);
}

string MiniPath::getCurrentDir()
{
    char cCurrentPath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        return 0;
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
    return string (cCurrentPath);
}

void MiniPath::fromNameInCurrentDir( const string& file_name )
{
    path = getCurrentDir();
    name = file_name;
}

string MiniPath::getName() const
{
    return name;
}

string MiniPath::getPath() const
{
    return path;
}

vector<string> MiniPath::getPathTokens() const
{
    if( !getDelim().empty() ) 
        return stringSplit( path + getDelim(), getDelim()[0] );

    vector<string> tmp;
    return tmp;
}

void MiniPath::setName( const string& name )
{
    string tmp;
    unsigned last_delim_pos = name.find_last_of ('/');
    if( last_delim_pos != string::npos )
        tmp = name.substr (last_delim_pos+1);
    else
        tmp = name;

    unsigned last_delim_pos2 = tmp.find_last_of ('\\');
    if( last_delim_pos2 != string::npos )
        tmp = tmp.substr (last_delim_pos2+1);

    this->name = tmp;
}

bool MiniPath::setPath( const string& absolut_path )
{
    if (isAbsoluteFilePath( absolut_path ))
    {
        path = absolut_path;
        return true;
    }
    return false;
}

bool MiniPath::isAbsoluteFilePath( const string& s )
{
    if (s.size() > 0)
    {
        if (s.at(0) == '/' || s.at(1) == ':')
            return true;
        else
            return false;
    }
    else
        return false;
}

std::list<string> MiniPath::listDirectories( const string& s )
{
    list<string> directories;

    struct _finddata_t c_file;
    intptr_t hFile;

    if( (hFile = _findfirst( "*.*", &c_file )) == -1L )
    {}
    else
    {
        do 
        {  
            char buffer[256];  
            if( ( c_file.attrib & _A_SUBDIR ) )
                directories.push_back( string( c_file.name ) );
        } while( _findnext( hFile, &c_file ) == 0 );        
    }

    return directories;
}

std::list<string> MiniPath::listFiles( const string& s, string filter )
{
    list<string> files;

    struct _finddata_t c_file;
    intptr_t hFile;

    if( (hFile = _findfirst( filter.c_str(), &c_file )) == -1L )
    {}
    else
    {
        do 
        {  
            char buffer[256];  
            if( !( c_file.attrib & _A_SUBDIR ) )
                files.push_back( string( c_file.name ) );
        } while( _findnext( hFile, &c_file ) == 0 );        
    }

    return files;
}

bool fileIOWindow(
    string& file_path,
	std::vector<string>& recently_used_files,
    const string& button_text,
    ImVec2 size )
{
    bool close_it = false;

    std::vector<const char*> extension_cstrings { "*.usr", "*.*" };

    static char current_folder[ 2048 ] = "x";
    if( strcmp( current_folder, "x" ) == 0 )
        strcpy( current_folder, MiniPath::getCurrentDir().c_str() );
    static char current_file[ 256 ] = "Test.usr";
    static int  file_type_selected = 0;
    static int  file_selected = 0;
    static int  directory_selected = 0;
    static bool directory_browsing = false;
    static int  recent_selected = 0;

    string tmp = string( current_folder );
    MiniPath current_mini_path( tmp );

    list<string> directories = MiniPath::listDirectories( file_path );
    std::vector<const char*> dir_list;
    for( const string& s : directories )
        dir_list.push_back( s.c_str() );
    
    list<string> local_files = MiniPath::listFiles( file_path, string( extension_cstrings[file_type_selected] ) );
    std::vector<const char*> file_list; 
    for( const string& s : local_files )
        file_list.push_back( s.c_str() );

    if ( directory_browsing ) 
        size.y += std::min( size_t( 8 ), std::max( dir_list.size(), file_list.size() ) ) *  GetFontSize();

    SetNextWindowSize( size );
    Begin( "FileIO" );

    Text("Directory: "); SameLine();
    PushItemWidth( GetWindowWidth() - 145 ); 

    InputText( " ", current_folder, IM_ARRAYSIZE( current_folder ) ); SameLine();

    std::vector<const char*> recent {};
    for( const auto& string : recently_used_files )
        recent.push_back(string.c_str());

    if( Button(" " CARET_DOWN " ") )
        ImGui::OpenPopup("RecentFiles");

    if (ImGui::BeginPopup("RecentFiles"))
    {
        if( ListBox( "", &recent_selected, recent.data(), recent.size() ) )
        {
            strcpy(current_file, recent[recent_selected]);
            file_path = current_file;
            CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    Text("           "); SameLine();

    vector<string> split_directories = current_mini_path.getPathTokens();
    for( int i = 0; i < split_directories.size(); ++i )
    {
        Button( split_directories[i].c_str() ); 
        if( i != split_directories.size()-1 )
            SameLine();
    }

    if( CollapsingHeader("Browse Directories") )
    {
        directory_browsing = true;
        Text( "           " );
        SameLine();

        PushItemWidth( GetWindowWidth()/2 - 60 );
        if( ListBox( " ", &directory_selected, dir_list.data(), dir_list.size() ) )
        {
            strcpy( current_folder, dir_list[directory_selected] );
        }

        SameLine();
        PushItemWidth( GetWindowWidth()/2 - 60 );
        if( ListBox( "", &file_selected, file_list.data(), file_list.size() ) )
        {
            strcpy( current_file, file_list[file_selected] );
        }
    }
    else
         directory_browsing = false;

    Text(" ");

    Text("File Name: "); SameLine();
    InputText( "  ", current_file, IM_ARRAYSIZE( current_file ) );

    Text( "File Type: " ); SameLine(); Text( extension_cstrings[file_type_selected] ); SameLine();    
    if (Button( CARET_DOWN ) )
        ImGui::OpenPopup("FileType");

    if (ImGui::BeginPopup("FileType"))
    {
        if( ListBox( "", &file_type_selected, extension_cstrings.data(), extension_cstrings.size() ) )
        {
            CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImVec2 pos = GetCursorPos();

    pos.x += GetWindowWidth() - 75 - button_text.size() * ( GetFontSize() * 0.7 );
    pos.y  = GetWindowHeight() - 30;

    SetCursorPos( pos );
    if( Button( button_text.c_str() ) )
    {
        file_path = current_file;
        close_it = true;
    }

    SameLine();
    if( Button( "Cancel" ) )
        close_it = true;
    End();

    return close_it;
}
