# Imgui-IGS-Snippets
Imgui code exchanges for convenience at IGS 

## FileIO Dialogue
 * similar to the standard windows file-io
 * additional recent file drop down
 * additional subdirectory buttons (linux style)
 
 ![Imgui FileIO Dialogue](images/Imgui-File-Open.PNG)
 
  * snippet example - save:
  ```c++
if( window_fileIO_visible )
{
    string save_file;
    if( fileIOWindow( save_file, window_recent_files, "Save", {"*.usr", "*.*"} ) )
    {
        window_fileIO_visible = false;
        if( !save_file.empty() )
        {
            window_recent_files.push_back( save_file );
 
            ofstream out_file;
            out_file.open( save_file, ios_base::trunc );          
            writeStuffToFile( out_file ); 
            out_file.close();
        }
    }
}  
  ```
    * snippet example - open:
	```c++
if( window_fileIO_visible )
{
    string open_file;
    if( fileIOWindow( open_file, window_recent_files, "Open", {"*.usr", "*.*"}, true  ) )
    {
        window_fileIO_visible = false;
        if( !open_file.empty() )
        {
            window_recent_files.push_back( open_file );
            readStuffFromFile( open_file );
        }
    }
}	
	```
  

## About Dialoge
 * text
 * close button
 * animated IGS logo
 
 ![Imgui About Dialogue](images/Imgui-About-IGS.PNG)
 
 * snippet example:
  ```c++
if( about_window_visible &&  
    aboutWindow( 
		ICON_FA_INFO_CIRCLE " About",
		"My fancy tools\n"
		"Abla bli blub ....\n",
		ImVec2( 400, 155 ),
		time ) )
		about_window_visible = false;
  ```