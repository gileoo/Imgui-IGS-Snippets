#ifndef SVGMINI_HPP
#define SVGMINI_HPP

#include <string>
#include <vector>

namespace svg
{

	struct PointStyle
	{
		float radius;
		float stroke_width;
		std::string fill_color;
		std::string stroke_color;

		PointStyle::PointStyle( float r, float sw, std::string fc, std::string sc );
	};

	const PointStyle BluePoint     ( 0.75f, 0.25f, "#5b9bd5", "#41719c" );
	const PointStyle PinkLinePoint ( 0.06f, 0.05f, "#ff4be4", "#ff4be4" );

	std::string
		CreatePoints(
			const std::vector<glm::vec3>& pts,
			PointStyle sps = BluePoint );

	std::string
		CreateLine(
			const std::vector<glm::vec3>& pts,
			int line_no = 0,
			float width = 0.25f,
			std::string line_color = "#b2194c" );

	std::string
		CreateText(
			const std::string& txt,
			int txt_no = 0,
			float size = 1.5,
			float x = 1.0,
			float y = 1.0 );

	std::string
		WrapContent(
			const std::string& txt,
			float height = 50.0f,
			float width = 50.0f );

	bool 
		SaveAsASCII( 
			const std::string& file_path,
			const std::string& txt );
}

#endif
