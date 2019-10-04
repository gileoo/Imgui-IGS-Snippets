#include <iostream>
#include <fstream> 
#include <sstream>
#include <iomanip>
#include <limits>

#include "Helper.hpp"

using namespace std;
using namespace glm;

vector<string> svg::split( 
	string txt, 
	char delimiter )
{
	string tmp;
	stringstream ss( txt );
	vector<string> tokens;

	while( getline( ss, tmp, delimiter ) )
		tokens.push_back(tmp);

	return tokens;
}

svg::PointStyle::PointStyle( 
	float r, 
	float sw, 
	string fc, 
	string sc )
	: radius( r ) 
	, stroke_width( sw )
	, fill_color( fc )
	, stroke_color( sc )
{}


string svg::CreatePoints( 
	const vector<vec3>& pts,
	svg::PointStyle sps )
{
	stringstream tmp( "" );

	for (int i = 0; i < pts.size(); ++i)
		tmp << setprecision( 3 )
		    << "  <circle id=\"circle" << i  
			<< "\" cx = \"" << pts[i].x
			<< "\" cy = \"" << pts[i].y
			<< "\" r = \""  << sps.radius
			<< "\" style=\"fill:"<< sps.fill_color << "; fill-opacity:1; stroke:"
			<< sps.stroke_color << "; stroke-width:"
			<< sps.stroke_width << "; stroke-miterlimit:4; stroke-dasharray:none; stroke-opacity:1\" />" << endl;

	return tmp.str();
}


string svg::CreateLine(
	const vector<vec3>& pts,
	int line_no,
	float width,
	string line_color )
{
	stringstream tmp( "" );

	if (pts.empty())
		return "";

	tmp << setprecision( 3 )
		<< "  <path" << endl
		<< "    id= \"path"<< line_no << "\"" << endl
		<< "    d=\""; 

	tmp << "M " << pts[0].x << "," << pts[0].y;

	for( int i = 1; i < pts.size(); ++i )
		tmp << " L " << pts[i].x << "," << pts[i].y;

	tmp << "\"" << endl;

	tmp << "    style=\"fill:none;stroke:" << line_color 
		<< ";stroke-width:" << width
		<< "px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1\" />"
		<< endl;

	return tmp.str();
}

string svg::CreateText(
	const string& txt,
	int txt_no,
	float size,
	float x,
	float y )
{
	stringstream tmp( "" );

	float xs = x * size;
	float ys = y * size;

	tmp << "  <text" << endl
		<< "    id = \"text" << txt_no << "\"" << endl
		<< "    y = \"" << ys << "\"" << endl
		<< "    x = \"" << xs << "\"" << endl
		<< "    style = \"font-style:normal;font-variant:normal;font-weight:normal;font-stretch:normal;font-size:"
		<< size << "px;line-height:1.1;font-family:'Times New Roman';-inkscape-font-specification:'Times New Roman, ';letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:0.26383314\"" << endl
		<< "    xml:space = \"preserve\">" << endl;

	vector<string> toks = svg::split( txt, '\n' );

	for( size_t l = 0; l < toks.size(); ++l )
	{
		tmp << "    <tspan" << endl
			<< "      style = \"stroke-width:0.26383314\"" << endl
			<< "      y = \"" << ys + l * size << "\"" << endl
			<< "      x = \"" << xs << "\"" << endl
			<< "      id = \"tspan" << l <<"\">" << toks[l] << "</tspan>" << endl;
	}

	tmp << "  </text>" << endl;

	return tmp.str();
}

string svg::WrapContent( 
	const string& txt, 
	float height, 
	float width )
{
	stringstream tmp("");

	tmp << "<svg width=\"" << width
		<< "mm\" height=\"" << height
		<< "mm\" viewBox=\"0 0 " 
		<< int(width) << " " << int(height) 
		<< "\" xmlns = \"http://www.w3.org/2000/svg\">" << endl;
	
	tmp << txt << endl;

	tmp << "</svg>" << endl;
	
	return tmp.str();
}

bool svg::SaveAsASCII( 
	const string& file_path,
	const string& txt )
{
	ofstream out_file( file_path );
	if (out_file.is_open())
	{
		out_file << txt;
		out_file.close();

		return true;
	}

	out_file.close();

	return false;
}