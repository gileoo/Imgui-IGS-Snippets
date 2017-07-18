# Imgui-IGS-Snippets
Imgui code exchanges for convenience at IGS 

## FileIO Dialogue
 * similar to the standard windows file-io
 * additional recent file drop down
 * additional subdirectory buttons (linux style)
 
 ![Imgui FileIO Dialogue](images/Imgui-File-Open.PNG)
 
  * example - save:
    inside the render loop somewhere do:
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
  
  

## About Dialoge
 * text
 * close button
 * animated IGS logo
 
 ![Imgui About Dialogue](images/Imgui-About-IGS.PNG)