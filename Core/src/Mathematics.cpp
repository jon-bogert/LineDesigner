/*========================================================

 Originally Created for "REngine" by Erick Tavares. Edited and adaped by Jon Bogert

 XephTools - Math

 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:

 1. The origin of this software must not be misrepresented;
	you must not claim that you wrote the original software.
	If you use this software in a product, an acknowledgment
	in the product documentation would be appreciated but is not required.

 2. Altered source versions must be plainly marked as such,
	and must not be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.

========================================================*/

#include "Mathematics.h"

#include <iostream>
#include <math.h>
#include <random>
#include <sstream>
#include <iomanip>

namespace xe
{
	//====================================================================================================
	// Constants
	//====================================================================================================

	constexpr Color Color::AliceBlue{ 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f };
	constexpr Color Color::AntiqueWhite{ 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f };
	constexpr Color Color::Aqua{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::Aquamarine{ 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f };
	constexpr Color Color::Azure{ 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::Beige{ 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f };
	constexpr Color Color::Bisque{ 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f };
	constexpr Color Color::Black{ 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::BlanchedAlmond{ 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f };
	constexpr Color Color::Blue{ 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::BlueViolet{ 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f };
	constexpr Color Color::Brown{ 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f };
	constexpr Color Color::BurlyWood{ 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f };
	constexpr Color Color::CadetBlue{ 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f };
	constexpr Color Color::Chartreuse{ 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::Chocolate{ 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f };
	constexpr Color Color::Coral{ 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f };
	constexpr Color Color::CornflowerBlue{ 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f };
	constexpr Color Color::Cornsilk{ 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f };
	constexpr Color Color::Crimson{ 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f };
	constexpr Color Color::Cyan{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::DarkBlue{ 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f };
	constexpr Color Color::DarkCyan{ 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f };
	constexpr Color Color::DarkGoldenrod{ 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f };
	constexpr Color Color::DarkGrey{ 0.2000000000f, 0.2000000000f, 0.2000000000f, 1.000000000f };
	constexpr Color Color::DarkGreen{ 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f };
	constexpr Color Color::DarkKhaki{ 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f };
	constexpr Color Color::DarkMagenta{ 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f };
	constexpr Color Color::DarkOliveGreen{ 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f };
	constexpr Color Color::DarkOrange{ 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f };
	constexpr Color Color::DarkOrchid{ 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f };
	constexpr Color Color::DarkRed{ 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::DarkSalmon{ 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f };
	constexpr Color Color::DarkSeaGreen{ 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f };
	constexpr Color Color::DarkSlateBlue{ 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f };
	constexpr Color Color::DarkSlateGrey{ 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f };
	constexpr Color Color::DarkTurquoise{ 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f };
	constexpr Color Color::DarkViolet{ 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f };
	constexpr Color Color::DeepPink{ 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f };
	constexpr Color Color::DeepSkyBlue{ 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f };
	constexpr Color Color::DimGrey{ 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f };
	constexpr Color Color::DodgerBlue{ 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f };
	constexpr Color Color::Firebrick{ 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f };
	constexpr Color Color::FloralWhite{ 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f };
	constexpr Color Color::ForestGreen{ 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f };
	constexpr Color Color::Fuchsia{ 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::Gainsboro{ 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f };
	constexpr Color Color::GhostWhite{ 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f };
	constexpr Color Color::Gold{ 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f };
	constexpr Color Color::Goldenrod{ 0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f };
	constexpr Color Color::Grey{ 0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f };
	constexpr Color Color::Green{ 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f };
	constexpr Color Color::GreenYellow{ 0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f };
	constexpr Color Color::Honeydew{ 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f };
	constexpr Color Color::HotPink{ 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f };
	constexpr Color Color::IndianRed{ 0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f };
	constexpr Color Color::Indigo{ 0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f };
	constexpr Color Color::Ivory{ 1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f };
	constexpr Color Color::Khaki{ 0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f };
	constexpr Color Color::Lavender{ 0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f };
	constexpr Color Color::LavenderBlush{ 1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f };
	constexpr Color Color::LawnGreen{ 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f };
	constexpr Color Color::LemonChiffon{ 1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f };
	constexpr Color Color::LightBlue{ 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f };
	constexpr Color Color::LightCoral{ 0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f };
	constexpr Color Color::LightCyan{ 0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::LightGoldenrodYellow{ 0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f };
	constexpr Color Color::LightGreen{ 0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f };
	constexpr Color Color::LightGrey{ 0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f };
	constexpr Color Color::LightPink{ 1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f };
	constexpr Color Color::LightSalmon{ 1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f };
	constexpr Color Color::LightSeaGreen{ 0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f };
	constexpr Color Color::LightSkyBlue{ 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f };
	constexpr Color Color::LightSlateGrey{ 0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f };
	constexpr Color Color::LightSteelBlue{ 0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f };
	constexpr Color Color::LightYellow{ 1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f };
	constexpr Color Color::Lime{ 0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::LimeGreen{ 0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f };
	constexpr Color Color::Linen{ 0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f };
	constexpr Color Color::Magenta{ 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::Maroon{ 0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::MediumAquamarine{ 0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f };
	constexpr Color Color::MediumBlue{ 0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f };
	constexpr Color Color::MediumOrchid{ 0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f };
	constexpr Color Color::MediumPurple{ 0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f };
	constexpr Color Color::MediumSeaGreen{ 0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f };
	constexpr Color Color::MediumSlateBlue{ 0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f };
	constexpr Color Color::MediumSpringGreen{ 0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f };
	constexpr Color Color::MediumTurquoise{ 0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f };
	constexpr Color Color::MediumVioletRed{ 0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f };
	constexpr Color Color::MidnightBlue{ 0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f };
	constexpr Color Color::MintCream{ 0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f };
	constexpr Color Color::MistyRose{ 1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f };
	constexpr Color Color::Moccasin{ 1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f };
	constexpr Color Color::NavajoWhite{ 1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f };
	constexpr Color Color::Navy{ 0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f };
	constexpr Color Color::OldLace{ 0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f };
	constexpr Color Color::Olive{ 0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f };
	constexpr Color Color::OliveDrab{ 0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f };
	constexpr Color Color::Orange{ 1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f };
	constexpr Color Color::OrangeRed{ 1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f };
	constexpr Color Color::Orchid{ 0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f };
	constexpr Color Color::PaleGoldenrod{ 0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f };
	constexpr Color Color::PaleGreen{ 0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f };
	constexpr Color Color::PaleTurquoise{ 0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f };
	constexpr Color Color::PaleVioletRed{ 0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f };
	constexpr Color Color::PapayaWhip{ 1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f };
	constexpr Color Color::PeachPuff{ 1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f };
	constexpr Color Color::Peru{ 0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f };
	constexpr Color Color::Pink{ 1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f };
	constexpr Color Color::Plum{ 0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f };
	constexpr Color Color::PowderBlue{ 0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f };
	constexpr Color Color::Purple{ 0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f };
	constexpr Color Color::Red{ 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::RosyBrown{ 0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f };
	constexpr Color Color::RoyalBlue{ 0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f };
	constexpr Color Color::SaddleBrown{ 0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f };
	constexpr Color Color::Salmon{ 0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f };
	constexpr Color Color::SandyBrown{ 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f };
	constexpr Color Color::SeaGreen{ 0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f };
	constexpr Color Color::SeaShell{ 1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f };
	constexpr Color Color::Sienna{ 0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f };
	constexpr Color Color::Silver{ 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f };
	constexpr Color Color::SkyBlue{ 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
	constexpr Color Color::SlateBlue{ 0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f };
	constexpr Color Color::SlateGrey{ 0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f };
	constexpr Color Color::Snow{ 1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f };
	constexpr Color Color::SpringGreen{ 0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f };
	constexpr Color Color::SteelBlue{ 0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f };
	constexpr Color Color::Tan{ 0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f };
	constexpr Color Color::Teal{ 0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f };
	constexpr Color Color::Thistle{ 0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f };
	constexpr Color Color::Tomato{ 1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f };
	constexpr Color Color::Transparent{ 0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f };
	constexpr Color Color::Turquoise{ 0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f };
	constexpr Color Color::Violet{ 0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f };
	constexpr Color Color::Wheat{ 0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f };
	constexpr Color Color::White{ 1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	constexpr Color Color::WhiteSmoke{ 0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f };
	constexpr Color Color::Yellow{ 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
	constexpr Color Color::YellowGreen{ 0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f };

	

	//====================================================================================================
	// Function Definitions
	//====================================================================================================


	Matrix4 MatrixRotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Math::Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return Matrix4
		(
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}

	//----------------------------------------------------------------------------------------------------

	Matrix4 QuaternionToMatrix(const Quaternion& q)
	{
		return Matrix4
		(
			1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
			(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
			(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
			0.0f,

			(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
			1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
			(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
			0.0f,

			(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
			(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
			1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
			0.0f,

			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}

	//----------------------------------------------------------------------------------------------------

	Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
	{
		Quaternion q_0{ Math::Normalize(q0) };
		Quaternion q_1{ Math::Normalize(q1) };
		//Find the dot product 
		float dot = (q_0.x * q_1.x) + (q_0.y * q_1.y) + (q_0.z * q_1.z) + (q_0.w * q_1.w);

		//Determine the direction of the rotation 
		if (dot < 0.0f)
		{
			dot = -dot;
			q_1.x = -q_1.x;
			q_1.y = -q_1.y;
			q_1.z = -q_1.z;
			q_1.w = -q_1.w;
		}
		else if (dot > 0.9995f)
		{
			return (Math::Lerp(q_0, q_1, t));
		}

		float theta_0 = acos(dot);
		float theta = theta_0 * t;
		float sinTheta = sin(theta);
		float sinTheta_0 = sin(theta_0);

		float s0 = cos(theta) - dot * sinTheta / sinTheta_0;
		float s1 = sinTheta / sinTheta_0;

		return (q_0 * s0) + (q_1 * s1);
	}
	//----------------------------------------------------------------------------------------------------
	Quaternion EulerToQuaternion(float x, float y, float z)
	{
		Quaternion q;
		float sx, sy, sz;
		float cx, cy, cz;
		sx = sinf(x * Math::kDegToRad * 0.5f); sy = sinf(y * Math::kDegToRad * 0.5f); sz = sinf(z * Math::kDegToRad * 0.5f);
		cx = cosf(x * Math::kDegToRad * 0.5f); cy = cosf(y * Math::kDegToRad * 0.5f); cz = cosf(z * Math::kDegToRad * 0.5f);

		q.w = (cx * cy * cz) + (sx * sy * sz);
		q.x = (sx * cy * cz) + (cx * sy * sz);
		q.y = (cx * sy * cz) - (sx * cy * sz);
		q.z = (cx * cy * sz) - (sx * sy * cz);


		q = Math::Normalize(q);
		return q;
	}

	//----------------------------------------------------------------------------------------------------

	Vector3 QuaternionToEuler(const Quaternion& q1)
	{
		Vector3 eulerAngles;

		Quaternion q = Math::Normalize(q1);

		// roll (z-axis rotation)
		float sinr = +2.0f * (q.w * q.x + q.y * q.z);
		float cosr = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
		eulerAngles.z = atan2(sinr, cosr) * Math::kRadToDeg;

		// pitch (x-axis rotation)
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
		{
			eulerAngles.x = static_cast<float>(copysign(Math::kPi * 0.5f, sinp)) * Math::kRadToDeg; // use 90 degrees if out of range
		}
		else
		{
			eulerAngles.x = static_cast<float>(asin(sinp)) * Math::kRadToDeg;
		}

		// yaw (y-axis rotation)
		float siny = +2.0f * (q.w * q.z + q.x * q.y);
		float cosy = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		eulerAngles.y = atan2(siny, cosy) * Math::kRadToDeg;


		return eulerAngles;
	}
	//----------------------------------------------------------------------------------------------------
	namespace Math
	{
		Quaternion Conjugate(Quaternion q0)
		{
			Quaternion q1;
			q1.w = q0.w;
			q1.x = -q0.x;
			q1.y = -q0.y;
			q1.z = -q0.z;

			return q1;
		}

		//----------------------------------------------------------------------------------------------------
		Quaternion Inverse(Quaternion q0)
		{
			Quaternion qInverse = Conjugate(q0) / Math::MagnitudeSqr(q0);

			return qInverse;
		}

		//----------------------------------------------------------------------------------------------------
		Quaternion LookRotation(const Vector3& look, const Vector3& up)
		{
			Vector3 forward = Math::Normalize(look);
			Vector3 vector2 = Math::Normalize(Math::Cross(up, forward));
			Vector3 vector3 = Math::Cross(forward, vector2);

			float m00 = vector2.x;
			float m01 = vector2.y;
			float m02 = vector2.z;
			float m10 = vector3.x;
			float m11 = vector3.y;
			float m12 = vector3.z;
			float m20 = forward.x;
			float m21 = forward.y;
			float m22 = forward.z;

			float num8 = (m00 + m11) + m22;

			Quaternion quaternion;

			if (num8 > 0.0f)
			{
				float num = Math::Sqrt(num8 + 1.0f);
				quaternion.w = num * 0.5f;
				num = 0.5f / num;
				quaternion.x = (m12 - m21) * num;
				quaternion.y = (m20 - m02) * num;
				quaternion.z = (m01 - m10) * num;

				return quaternion;
			}

			if ((m00 >= m11) && (m00 >= m22))
			{
				float num7 = Math::Sqrt(((1.0f + m00) - m11) - m22);
				float num4 = 0.5f / num7;
				quaternion.x = 0.5f * num7;
				quaternion.y = (m01 + m10) * num4;
				quaternion.z = (m02 + m20) * num4;
				quaternion.w = (m12 - m21) * num4;
				return quaternion;
			}

			if (m11 > m22)
			{
				float num6 = Math::Sqrt(((1.0f + m11) - m00) - m22);
				float num3 = 0.5f / num6;
				quaternion.x = (m10 + m01) * num3;
				quaternion.y = 0.5f * num6;
				quaternion.z = (m21 + m12) * num3;
				quaternion.w = (m20 - m02) * num3;
				return quaternion;
			}

			float num5 = Math::Sqrt(((1.0f + m22) - m00) - m11);
			float num2 = 0.5f / num5;

			quaternion.x = (m20 + m02) * num2;
			quaternion.y = (m21 + m12) * num2;
			quaternion.z = 0.5f * num5;
			quaternion.w = (m01 - m10) * num2;
			return quaternion;

		}
		//----------------------------------------------------------------------------------------------------
		Quaternion QuaternionFromAxisAngle(const Vector3& axis, float angleDegrees)
		{
			Quaternion q;
			float angleRadians = angleDegrees * Math::kDegToRad;
			float s = sin(angleRadians * 0.5f);

			q.x = axis.x * s;
			q.y = axis.y * s;
			q.z = axis.z * s;
			q.w = cos(angleRadians * 0.5f);

			return q;
		}

		//----------------------------------------------------------------------------------------------------

		Quaternion FromToQuaternion(const Vector3& from, const Vector3& to)
		{
			Vector3 normalizedFrom = Math::Normalize(from);
			Vector3 normalizedTo = Math::Normalize(to);

			float c = Math::Dot(normalizedFrom, normalizedTo);
			float angle = acos(c);
			Vector3 w = Math::Normalize(Math::Cross(from, to));
			return QuaternionFromAxisAngle(w, angle);
		}

		//----------------------------------------------------------------------------------------------------
		Quaternion QuaternionFromMatrix(const Matrix4& mat)
		{
			Matrix4 rotationMatrix = Math::Transpose(mat);
			Quaternion q;

			float trace{ rotationMatrix._11 + rotationMatrix._22 + rotationMatrix._33 };

			if (trace > 0.0f)
			{
				float s = 0.5f / sqrt(trace + 1.0f);

				q.w = 0.25f / s;
				q.x = (rotationMatrix._32 - rotationMatrix._23) * s;
				q.y = (rotationMatrix._13 - rotationMatrix._31) * s;
				q.z = (rotationMatrix._21 - rotationMatrix._12) * s;
			}
			else
			{
				if (rotationMatrix._11 > rotationMatrix._22 && rotationMatrix._11 > rotationMatrix._33)
				{
					float s = 2.0f * sqrt(1.0f + rotationMatrix._11 - rotationMatrix._22 - rotationMatrix._33);
					q.x = 0.25f * s;
					q.y = (rotationMatrix._12 + rotationMatrix._21) / s;
					q.z = (rotationMatrix._13 + rotationMatrix._31) / s;
					q.w = (rotationMatrix._32 - rotationMatrix._23) / s;
				}
				else if (rotationMatrix._22 > rotationMatrix._33)
				{
					float s = 2.0f * sqrt(1.0f + rotationMatrix._22 - rotationMatrix._11 - rotationMatrix._33);
					q.x = (rotationMatrix._12 + rotationMatrix._21) / s;
					q.y = 0.25f * s;
					q.z = (rotationMatrix._23 + rotationMatrix._32) / s;
					q.w = (rotationMatrix._13 - rotationMatrix._31) / s;
				}
				else
				{
					float s = 2.0f * sqrt(1.0f + rotationMatrix._33 - rotationMatrix._11 - rotationMatrix._22);

					q.x = (rotationMatrix._13 + rotationMatrix._31) / s;
					q.y = (rotationMatrix._23 + rotationMatrix._32) / s;
					q.z = 0.25f * s;
					q.w = (rotationMatrix._21 - rotationMatrix._12) / s;
				}
			}

			return Math::Normalize(q);
		}

		//----------------------------------------------------------------------------------------------------

		bool PointInRect(const Vector2& point, const Rect& rect)
		{
			if (point.x > rect.right || point.x < rect.left ||
				point.y > rect.bottom || point.y < rect.top)
			{
				return false;
			}
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool PointInCircle(const Vector2& point, const Circle& circle)
		{
			const Vector2 centerToPoint = point - circle.center;
			const float distSqr = Math::Dot(centerToPoint, centerToPoint);
			const float radiusSqr = circle.radius * circle.radius;
			return distSqr < radiusSqr;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const LineSegment& a, const LineSegment& b)
		{
			// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

			float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
			float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
			float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

			// First check for special cases
			if (denom == 0.0f)
			{
				if (ua == 0.0f && ub == 0.0f)
				{
					// The line segments are the same
					return true;
				}
				else
				{
					// The line segments are parallel
					return false;
				}
			}

			ua /= denom;
			ub /= denom;

			if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
			{
				return false;
			}

			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Circle& c0, const Circle& c1)
		{
			const float totalRadius = c0.radius + c1.radius;
			const float totalRadiusSquare = totalRadius * totalRadius;
			const float fDistanceSquared = Math::DistanceSqr(c0.center, c1.center);
			return (fDistanceSquared < totalRadiusSquare);
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Rect& r0, const Rect& r1)
		{
			if (r0.left > r1.right)
				return false;
			else if (r0.top > r1.bottom)
				return false;
			else if (r0.right < r1.left)
				return false;
			else if (r0.bottom < r1.top)
				return false;
			return true;
		}
		//----------------------------------------------------------------------------------------------------

		bool Intersect(const AABB2D& aabb0, const AABB2D& aabb1)
		{
			if (aabb0.Xmin > aabb1.Xmax)
				return false;
			else if (aabb0.Ymin > aabb1.Ymax)
				return false;
			else if (aabb0.Xmax < aabb1.Xmin)
				return false;
			else if (aabb0.Ymax < aabb1.Ymin)
				return false;
			return true;
		}

		//----------------------------------------------------------------------------------------------------


		bool Intersect(const Circle& c, const LineSegment& l)
		{
			Vector2 startToCenter = c.center - l.from;
			Vector2 startToEnd = l.to - l.from;
			float len = Math::Magnitude(startToEnd);
			Vector2 dir = startToEnd / len;

			// Find the closest point to the line segment
			float projection = Math::Dot(startToCenter, dir);
			Vector2 closestPoint;
			if (projection > len)
			{
				closestPoint = l.to;
			}
			else if (projection < 0.0f)
			{
				closestPoint = l.from;
			}
			else
			{
				closestPoint = l.from + (dir * projection);
			}

			// Check if the closest point is within the circle
			Vector2 closestToCenter = c.center - closestPoint;
			if (Math::MagnitudeSqr(closestToCenter) > Math::Sqr(c.radius))
			{
				return false;
			}
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const LineSegment& l, const Circle& c)
		{
			return Intersect(c, l);
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Rect& r, const Circle& c)
		{
			Vector2 closestPoint;
			closestPoint.x = Math::Clamp(c.center.x, r.left, r.right);
			closestPoint.y = Math::Clamp(c.center.y, r.top, r.bottom);

			const float distance = Math::Distance(closestPoint, c.center);
			if (distance > c.radius)
			{
				return false;
			}
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Circle& c, const Rect& r)
		{
			return Intersect(r, c);
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance)
		{
			// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

			// Find vectors for two edges sharing V1
			Vector3 e1 = b - a;
			Vector3 e2 = c - a;

			// Begin calculating determinant - also used to calculate u parameter
			Vector3 P = Math::Cross(ray.dir, e2);

			// If determinant is near zero, ray lies in plane of triangle
			float det = Math::Dot(e1, P);
			// NOT CULLING
			if (Math::IsZero(det))
			{
				return false;
			}

			float inv_det = 1.0f / det;

			// Calculate distance from V1 to ray origin
			Vector3 T = ray.org - a;

			// Calculate u parameter and test bound
			float u = Math::Dot(T, P) * inv_det;

			// The intersection lies outside of the triangle
			if (u < 0.0f || u > 1.0f)
			{
				return false;
			}

			// Prepare to test v parameter
			Vector3 Q = Math::Cross(T, e1);

			// Calculate V parameter and test bound
			float v = Math::Dot(ray.dir, Q) * inv_det;

			// The intersection lies outside of the triangle
			if (v < 0.0f || u + v  > 1.0f)
			{
				return false;
			}

			// Ray intersection
			float t = Math::Dot(e2, Q) * inv_det;
			if (t <= 0.0f)
			{
				// No hit, no win
				return false;
			}

			distance = t;
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Ray& ray, const Plane& plane, float& distance)
		{
			const float orgDotN = Math::Dot(ray.org, plane.n);
			const float dirDotN = Math::Dot(ray.dir, plane.n);

			// Check if ray is parallel to the plane
			if (Math::IsZero(dirDotN))
			{
				if (Math::IsZero(orgDotN - plane.d))
				{
					distance = 0.0f;
					return true;
				}
				else
				{
					return false;
				}
			}

			// Compute distance
			distance = (plane.d - orgDotN) / dirDotN;
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit)
		{
			// https://truesculpt.googlecode.com/hg-history/Release%25200.8/Doc/ray_box_intersect.pdf

			// Returns true if ray intersects bounding box
			// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

			Vector3 boxMin = aabb.center - aabb.extend;
			Vector3 boxMax = aabb.center + aabb.extend;
			float tmin, tmax, tymin, tymax, tzmin, tzmax;

			float divx = 1.0f / ray.dir.x;
			float divy = 1.0f / ray.dir.y;
			float divz = 1.0f / ray.dir.z;

			if (divx >= 0.0f)
			{
				tmin = (boxMin.x - ray.org.x) * divx;
				tmax = (boxMax.x - ray.org.x) * divx;
			}
			else
			{
				tmin = (boxMax.x - ray.org.x) * divx;
				tmax = (boxMin.x - ray.org.x) * divx;
			}
			if (divy >= 0.0f)
			{
				tymin = (boxMin.y - ray.org.y) * divy;
				tymax = (boxMax.y - ray.org.y) * divy;
			}
			else
			{
				tymin = (boxMax.y - ray.org.y) * divy;
				tymax = (boxMin.y - ray.org.y) * divy;
			}

			if ((tmin > tymax) || (tymin > tmax))
			{
				return false;
			}

			if (tymin > tmin)
			{
				tmin = tymin;
			}

			if (tymax < tmax)
			{
				tmax = tymax;
			}

			if (divz >= 0.0f)
			{
				tzmin = (boxMin.z - ray.org.z) * divz;
				tzmax = (boxMax.z - ray.org.z) * divz;
			}
			else
			{
				tzmin = (boxMax.z - ray.org.z) * divz;
				tzmax = (boxMin.z - ray.org.z) * divz;
			}

			if ((tmin > tzmax) || (tzmin > tmax))
			{
				return false;
			}

			if (tzmin > tmin)
			{
				tmin = tzmin;
			}

			if (tzmax < tmax)
			{
				tmax = tzmax;
			}

			distEntry = tmin;
			distExit = tmax;
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		bool Intersect(const Vector3& point, const AABB& aabb)
		{
			const Vector3 test = point - aabb.center;
			if (abs(test.x) > aabb.extend.x) return false;
			if (abs(test.y) > aabb.extend.y) return false;
			if (abs(test.z) > aabb.extend.z) return false;
			return true;
		}

		//----------------------------------------------------------------------------------------------------

		Rect GetAABB2DIntersectionRect(const AABB2D& aabb0, const AABB2D& aabb1)
		{
			Rect intersection;

			intersection.left = Max(aabb0.Xmin, aabb1.Xmin);
			intersection.top = Max(aabb0.Ymin, aabb1.Ymin);
			intersection.right = Min(aabb0.Xmax, aabb1.Xmax);
			intersection.bottom = Min(aabb0.Ymax, aabb1.Ymax);

			return intersection;
		}

		Vector3 GetClosestPoint(const Ray& ray, const Vector3& point)
		{
			Vector3 orgToPoint = point - ray.org;
			float d = Math::Dot(orgToPoint, ray.dir);
			return ray.org + (ray.dir * d);
		}

		//----------------------------------------------------------------------------------------------------

		Vector3 Mean(const Vector3* v, uint32_t count)
		{
			Vector3 mean(0.0f, 0.0f, 0.0f);
			for (uint32_t i = 0; i < count; ++i)
			{
				mean += v[i];
			}
			return mean / (float)count;
		}

		Vector3 VecMax(const Vector3& a, const Vector3& b)
		{
			Vector3 result;
			result = { std::max(a.x, b.x), std::max(a.y, b.y) , std::max(a.z, b.z) };
			return result;
		}

		Vector3 VecMin(const Vector3& a, const Vector3& b)
		{
			Vector3 result;
			result = { std::min(a.x, b.x), std::min(a.y, b.y) , std::min(a.z, b.z) };
			return result;
		}
		float Remap(float oldLower, float oldUpper, float newLower, float newUpper, float value)
		{
			float rangeFactor = (newUpper - newLower) / (oldUpper - oldLower);
			return (value - oldLower) * rangeFactor + newLower;
		}
		int Round(float num)
		{
			return static_cast<int>(num + 0.5f);
		}

		float Round(float num, int decPlace)
		{
			return static_cast<float>(static_cast<int>(num * 10 * decPlace + 0.5f) / (10.f * decPlace));
		}
		bool FloatComp(float a, float b)
		{
			return (a >= b - FLT_EPSILON && a <= b + FLT_EPSILON);
		}
		Math::Random::Random()
		{
			srand(time(0));
		}

		Math::Random& Math::Random::Get()
		{
			static Random inst;
			return inst;
		}
		int Math::Random::Range(int lower, int upper, float increment)
		{
			return ((rand() % (upper - lower)) + lower) * increment;
		}
		float Math::Random::Range(float lower, float upper, float increment)
		{
			int decPoint{};
			int intInc{};
			for (; decPoint <= 6; ++decPoint)
			{
				if (static_cast<int>(increment * 10 * decPoint) == increment * 10 * decPoint)
				{
					intInc = static_cast<int>(increment * 10 * decPoint);
					break;
				}
			}
			int intLow = static_cast<int>(lower * 10 * decPoint) / intInc;
			int intUp = static_cast<int>(upper * 10 * decPoint) / intInc;

			return (((rand() % (intUp - intLow)) + intUp) * intInc) / (10.f * decPoint);
		}

		uint32_t Math::Random::UInt32()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<uint32_t> dis(0, UINT32_MAX);
			return dis(gen);
		}
	}
	void Matrix3::SetScale(const Vector2& scale)
	{
		Vector2 xAxis(_11, _21);
		Vector2 yAxis(_12, _22);

		xAxis = xe::Math::Normalize(xAxis);
		yAxis = xe::Math::Normalize(yAxis);

		_11 = xAxis.x * scale.x;
		_21 = xAxis.y * scale.x;

		_12 = yAxis.x * scale.y;
		_22 = yAxis.y * scale.y;
	}
	Color::Color(const std::string& hex, float alpha)
	{
		uint32_t intVals = std::stoul(hex, nullptr, 16);
		uint8_t* bytes = (uint8_t*)(&intVals);

		*this = Color(bytes[2], bytes[1], bytes[0], (uint8_t)(alpha * 255));
	}
}