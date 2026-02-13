#pragma once

#include <iostream>
#include <vector>
#include <assert.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <imgui.h>

namespace xe
{

	struct Vector2
	{
		float x, y;

		Vector2() : x(0.0f), y(0.0f) {}
		constexpr Vector2(float inX, float inY) : x(inX), y(inY) {}

		static Vector2 Zero() { return Vector2(); }
		static Vector2 One() { return Vector2(1.0f, 1.0f); }
		static Vector2 XAxis() { return Vector2(1.0f, 0.0f); }
		static Vector2 YAxis() { return Vector2(0.0f, 1.0f); }

		Vector2 operator-() const { return Vector2(-x, -y); }
		Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
		Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
		Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
		Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

		Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
		Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
		Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
		Vector2& operator/=(float s) { x /= s; y /= s; return *this; }

		bool operator==(const Vector2& rhs) { return (x == rhs.x && y == rhs.y); }
		bool operator!=(const Vector2& rhs) { return (x != rhs.x || y != rhs.y); }

		friend std::ostream& operator<<(std::ostream& os, const Vector2& v2) { os << v2.ToString(); return os; } // **EDIT**
		std::string ToString() const { return "X:" + std::to_string(x) + " Y:" + std::to_string(y); }

		operator sf::Vector2f() const { return { x, y }; };							// **EDIT** Added for SFML Compatibility
		operator sf::Vector2i() const { return { (int)x, (int)y }; };				// **EDIT** Added for SFML Compatibility
		operator sf::Vector2u() const { return { (unsigned)x, (unsigned)y }; };		// **EDIT** Added for SFML Compatibility

		Vector2& operator=(const sf::Vector2f& rhs) { x = rhs.x; y = rhs.y; return *this; }		// **EDIT** Added for SFML Compatibility
		Vector2& operator=(const sf::Vector2i& rhs) { x = rhs.x; y = rhs.y; return *this; }		// **EDIT** Added for SFML Compatibility
		Vector2& operator=(const sf::Vector2u& rhs) { x = rhs.x; y = rhs.y; return *this; }		// **EDIT** Added for SFML Compatibility

		Vector2(const sf::Vector2f& other) : x(other.x), y(other.y) {}						// **EDIT** Added for SFML Compatibility
		Vector2(const sf::Vector2i& other) : x(other.x), y(other.y) {}						// **EDIT** Added for SFML Compatibility
		Vector2(const sf::Vector2u& other) : x(other.x), y(other.y) {}						// **EDIT** Added for SFML Compatibility

		operator ImVec2() const { return { x, y }; };										// **EDIT** Added for SFML Compatibility
		Vector2& operator=(const ImVec2& rhs) { x = rhs.x; y = rhs.y; return *this; }		// **EDIT** Added for SFML Compatibility
		Vector2(const ImVec2& other) : x(other.x), y(other.y) {}							// **EDIT** Added for SFML Compatibility
	};
	//----------------------------------------------------------------------------------------------------

	struct Vector3
	{
		float x, y, z;

		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

		static Vector3 Zero() { return Vector3(); }
		static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
		static Vector3 XAxis() { return Vector3(1.0f, 0.0f, 0.0f); }
		static Vector3 YAxis() { return Vector3(0.0f, 1.0f, 0.0f); }
		static Vector3 ZAxis() { return Vector3(0.0f, 0.0f, 1.0f); }

		Vector3 operator-() const { return Vector3(-x, -y, -z); }
		Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
		Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
		Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
		Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

		Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
		Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
		Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

		bool operator==(const Vector3& rhs)
		{
			if (x == 0.0f && y == 0.0f && z == 0.0f)
			{
				return true;
			}
			return false;
		}
		friend std::ostream& operator<<(std::ostream& os, const Vector3& v3) { os << v3.ToString(); return os; } // **EDIT**
		std::string ToString() const { return "X:" + std::to_string(x) + " Y:" + std::to_string(y) + " Z:" + std::to_string(z); }
		operator sf::Vector3f() const { return { x, y, z }; };						// **EDIT** Added for SFML Compatibility
		operator sf::Vector3i() const { return { (int)x, (int)y, (int)z }; };		// **EDIT** Added for SFML Compatibility
	};

	//----------------------------------------------------------------------------------------------------

	struct Vector4
	{
		float x, y, z, w;

		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vector4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}
		Vector4(Vector3 v, float inW) : x(v.x), y(v.y), z(v.z), w(inW) {}

		Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
		Vector4 operator+(const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		Vector4 operator-(const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
		Vector4 operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }
		Vector4 operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }

		Vector4& operator+=(const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
		Vector4& operator-=(const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
		Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
		Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

	};

	//----------------------------------------------------------------------------------------------------

	struct Quaternion
	{
		float x, y, z; //Imaginary Vector
		float w; //Real number

		Quaternion()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
			, w(0.0f)
		{}

		Quaternion(float x1, float y1, float z1, float w1)
		{
			x = x1;	y = y1;	z = z1;
			w = w1;
		}

		Quaternion(Vector3 xyz, float w1)
		{
			x = xyz.x; y = xyz.y; z = xyz.z;
			w = w1;
		}

		Quaternion(Vector4 xyzw)
		{
			x = xyzw.x; y = xyzw.y; z = xyzw.z;
			w = xyzw.w;
		}

		static Quaternion Zero() { return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); }
		static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

		Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		Quaternion operator-(const Quaternion& rhs) const { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
		Quaternion operator*(float s) const { return Quaternion(x * s, y * s, z * s, w * s); }
		Quaternion operator*(const Quaternion& rhs) const
		{
			return Quaternion(w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y
				, w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x
				, w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
				, w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);

		}

		Quaternion operator/(float s) const
		{
			//XE_ASSERT(s != 0.0f, "s can't be zero!");
			return Quaternion(x / s, y / s, z / s, w / s);
		}
	};

	struct Rotator
	{
	public:
		void SetRad(const float val)
		{
			m_val = val * 180.f / 3.14159265358979f;
			CheckVal();
		}
		float GetRad() const
		{
			return m_val * 3.14159265358979f / 180.f;
		}
		float AddRad(const float amt)
		{
			m_val += amt * 180.f / 3.14159265358979f;
			CheckVal();
			return m_val;
		}
		void SetDeg(const float val)
		{
			m_val = val;
			CheckVal();
		}
		float GetDeg() const
		{
			return m_val;
		}
		float AddDeg(const float amt)
		{
			m_val += amt;
			CheckVal();
			return m_val;
		}

	private:
		float m_val = 0.f;

		void CheckVal()
		{
			while (m_val > 180.f)
				m_val -= 360.f;
			while (m_val <= -180.f)
				m_val += 360.f;
		}
	};

	struct Color8
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;

		friend std::ostream& operator<<(std::ostream& os, const Color8& color)
		{
			os << "R:" << (int)color.r << " G:" << (int)color.g << " B:" << (int)color.b << " A:" << (int)color.a;
			return os;
		}
	};

	struct Color
	{
		float r = 0.f;
		float g = 0.f;
		float b = 0.f;
		float a = 1.f;

		Color() = default;
		Color(const Color&) = default;
		Color& operator=(const Color&) = default;
		Color& operator=(const std::string& hex) { *this = Color(hex); return *this; }

		constexpr Color(float r, float g, float b, float a) noexcept
			:r(r), g(g), b(b), a(a) {}
		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
			:r(r / 255.f), g(g / 255.f), b(b / 255.f), a(a / 255.f) {}
		Color(const std::string& hex, float alpha = 1.f);

		Color8 As8bit() const
		{
			return Color8{ uint8_t((r * 255) + 0.5f), uint8_t((g * 255) + 0.5f), uint8_t((b * 255) + 0.5f), uint8_t((a * 255) + 0.5f) };
		}

		constexpr Color operator-() const { return { -r, -g, -b, -a }; }
		constexpr Color operator+(Color other) const { return { r + other.r, g + other.g, b + other.b, a + other.a }; }
		constexpr Color operator-(Color other) const { return { r - other.r, g - other.g, b - other.b, a - other.a }; }
		constexpr Color operator*(float s) const { return { r * s, g * s, b * s, a * s }; }
		constexpr Color operator/(float s) const { return { r / s, g / s, b / s, a / s }; }

		constexpr Color& operator+=(const Color& v) { r += v.r; g += v.g; b += v.b; a += v.a; return *this; }
		constexpr Color& operator-=(const Color& v) { r -= v.r; g -= v.g; b -= v.b; a -= v.a; return *this; }
		constexpr Color& operator*=(float s) { r *= s; g *= s, b *= s, a *= s; return *this; }
		constexpr Color& operator/=(float s) { r /= s; g /= s, b /= s, a /= s; return *this; }

		friend std::ostream& operator<<(std::ostream& os, const Color& color)
		{
			os << "R:" << color.r << " G:" << color.g << " B:" << color.b << " A:" << color.a;
			return os;
		}

		operator sf::Color() const { return { static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255), static_cast<uint8_t>(a * 255) }; };		// **EDIT** Added for SFML Compatibility
		Color& operator=(const sf::Color& rhs) { r = rhs.r / 255.f; g = rhs.g / 255.f; b = rhs.b / 255.f; a = rhs.a / 255.f; return *this; }										// **EDIT** Added for SFML Compatibility
		Color(const sf::Color& other) : r(other.r / 255.f), g(other.g / 255.f), b(other.b / 255.f), a(other.a / 255.f) {}															// **EDIT** Added for SFML Compatibility

		static const Color AliceBlue;
		static const Color AntiqueWhite;
		static const Color Aqua;
		static const Color Aquamarine;
		static const Color Azure;
		static const Color Beige;
		static const Color Bisque;
		static const Color Black;
		static const Color BlanchedAlmond;
		static const Color Blue;
		static const Color BlueViolet;
		static const Color Brown;
		static const Color BurlyWood;
		static const Color CadetBlue;
		static const Color Chartreuse;
		static const Color Chocolate;
		static const Color Coral;
		static const Color CornflowerBlue;
		static const Color Cornsilk;
		static const Color Crimson;
		static const Color Cyan;
		static const Color DarkBlue;
		static const Color DarkCyan;
		static const Color DarkGoldenrod;
		static const Color DarkGrey;
		static const Color DarkGreen;
		static const Color DarkKhaki;
		static const Color DarkMagenta;
		static const Color DarkOliveGreen;
		static const Color DarkOrange;
		static const Color DarkOrchid;
		static const Color DarkRed;
		static const Color DarkSalmon;
		static const Color DarkSeaGreen;
		static const Color DarkSlateBlue;
		static const Color DarkSlateGrey;
		static const Color DarkTurquoise;
		static const Color DarkViolet;
		static const Color DeepPink;
		static const Color DeepSkyBlue;
		static const Color DimGrey;
		static const Color DodgerBlue;
		static const Color Firebrick;
		static const Color FloralWhite;
		static const Color ForestGreen;
		static const Color Fuchsia;
		static const Color Gainsboro;
		static const Color GhostWhite;
		static const Color Gold;
		static const Color Goldenrod;
		static const Color Grey;
		static const Color Green;
		static const Color GreenYellow;
		static const Color Honeydew;
		static const Color HotPink;
		static const Color IndianRed;
		static const Color Indigo;
		static const Color Ivory;
		static const Color Khaki;
		static const Color Lavender;
		static const Color LavenderBlush;
		static const Color LawnGreen;
		static const Color LemonChiffon;
		static const Color LightBlue;
		static const Color LightCoral;
		static const Color LightCyan;
		static const Color LightGoldenrodYellow;
		static const Color LightGreen;
		static const Color LightGrey;
		static const Color LightPink;
		static const Color LightSalmon;
		static const Color LightSeaGreen;
		static const Color LightSkyBlue;
		static const Color LightSlateGrey;
		static const Color LightSteelBlue;
		static const Color LightYellow;
		static const Color Lime;
		static const Color LimeGreen;
		static const Color Linen;
		static const Color Magenta;
		static const Color Maroon;
		static const Color MediumAquamarine;
		static const Color MediumBlue;
		static const Color MediumOrchid;
		static const Color MediumPurple;
		static const Color MediumSeaGreen;
		static const Color MediumSlateBlue;
		static const Color MediumSpringGreen;
		static const Color MediumTurquoise;
		static const Color MediumVioletRed;
		static const Color MidnightBlue;
		static const Color MintCream;
		static const Color MistyRose;
		static const Color Moccasin;
		static const Color NavajoWhite;
		static const Color Navy;
		static const Color OldLace;
		static const Color Olive;
		static const Color OliveDrab;
		static const Color Orange;
		static const Color OrangeRed;
		static const Color Orchid;
		static const Color PaleGoldenrod;
		static const Color PaleGreen;
		static const Color PaleTurquoise;
		static const Color PaleVioletRed;
		static const Color PapayaWhip;
		static const Color PeachPuff;
		static const Color Peru;
		static const Color Pink;
		static const Color Plum;
		static const Color PowderBlue;
		static const Color Purple;
		static const Color Red;
		static const Color RosyBrown;
		static const Color RoyalBlue;
		static const Color SaddleBrown;
		static const Color Salmon;
		static const Color SandyBrown;
		static const Color SeaGreen;
		static const Color SeaShell;
		static const Color Sienna;
		static const Color Silver;
		static const Color SkyBlue;
		static const Color SlateBlue;
		static const Color SlateGrey;
		static const Color Snow;
		static const Color SpringGreen;
		static const Color SteelBlue;
		static const Color Tan;
		static const Color Teal;
		static const Color Thistle;
		static const Color Tomato;
		static const Color Transparent;
		static const Color Turquoise;
		static const Color Violet;
		static const Color Wheat;
		static const Color White;
		static const Color WhiteSmoke;
		static const Color Yellow;
		static const Color YellowGreen;

	};

	//----------------------------------------------------------------------------------------------------

	struct Matrix3
	{
		float _11, _12, _13;
		float _21, _22, _23;
		float _31, _32, _33;

		Matrix3()
			: _11(0.0f), _12(0.0f), _13(0.0f)
			, _21(0.0f), _22(0.0f), _23(0.0f)
			, _31(0.0f), _32(0.0f), _33(0.0f)
		{}

		Matrix3(
			float _11, float _12, float _13,
			float _21, float _22, float _23,
			float _31, float _32, float _33)
			: _11(_11), _12(_12), _13(_13)
			, _21(_21), _22(_22), _23(_23)
			, _31(_31), _32(_32), _33(_33)
		{}

		static Matrix3 Zero() { return Matrix3(); }
		static Matrix3 Identity() { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix3 Translation(float x, float y) { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, 1.0f); }
		static Matrix3 Translation(const Vector2& v) { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, 1.0f); }
		static Matrix3 Rotation(float rad) { return Matrix3(cosf(rad), sinf(rad), 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix3 Scaling(float s) { return Matrix3(s, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix3 Scaling(float sx, float sy) { return Matrix3(sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix3 Scaling(const Vector2& s) { return Matrix3(s.x, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 1.0f); }
		
		Vector2 GetTranslation() const { return Vector2(_31, _32); }
		float GetRotation() const { return atan2f(_21, _11); }
		Vector2 GetScale() const
		{
			float scaleX = sqrtf(_11 * _11 + _21 * _21);
			float scaleY = sqrtf(_12 * _12 + _22 * _22);
			return Vector2(scaleX, scaleY);
		}

		void SetTranslation(const Vector2& v) { _31 = v.x, _32 = v.y; }
		void SetRotation(const float rad) { _11 = cosf(rad); _12 = sinf(rad); _21 = -sinf(rad); _22 = cosf(rad); }
		void SetScale(const Vector2& scale);

		Matrix3 operator-() const
		{
			return Matrix3(
				-_11, -_12, -_13,
				-_21, -_22, -_23,
				-_31, -_32, -_33);
		}
		Matrix3 operator+(const Matrix3& rhs) const
		{
			return Matrix3(
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33);
		}
		Matrix3 operator-(const Matrix3& rhs) const
		{
			return Matrix3(
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33);
		}
		Matrix3 operator*(const Matrix3& rhs) const
		{
			return Matrix3(
				(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31),
				(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32),
				(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33),

				(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31),
				(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32),
				(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33),

				(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31),
				(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32),
				(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33));
		}
		Matrix3 operator*(float s) const
		{
			return Matrix3(
				_11 * s, _12 * s, _13 * s,
				_21 * s, _22 * s, _23 * s,
				_31 * s, _32 * s, _33 * s);
		}
		Matrix3 operator/(float s) const
		{
			//ASSERT(s != 0.0f, "Cannot divide by 0!");
			const float inv = 1.0f / s;
			return (*this) * inv;
		}
		Matrix3 operator+=(const Matrix3& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33;
			return *this;
		}
	};

	//----------------------------------------------------------------------------------------------------

	struct Matrix4
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;

		Matrix4()
			: _11(0.0f), _12(0.0f), _13(0.0f), _14(0.0f)
			, _21(0.0f), _22(0.0f), _23(0.0f), _24(0.0f)
			, _31(0.0f), _32(0.0f), _33(0.0f), _34(0.0f)
			, _41(0.0f), _42(0.0f), _43(0.0f), _44(0.0f)
		{}

		Matrix4(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
			: _11(_11), _12(_12), _13(_13), _14(_14)
			, _21(_21), _22(_22), _23(_23), _24(_24)
			, _31(_31), _32(_32), _33(_33), _34(_34)
			, _41(_41), _42(_42), _43(_43), _44(_44)
		{}

		static Matrix4 Zero() { return Matrix4(); }
		static Matrix4 Identity() { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4 Translation(float x, float y, float z) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f); }
		static Matrix4 Translation(const Vector3& v) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, v.z, 1.0f); }
		static Matrix4 RotationX(float rad) { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4 RotationY(float rad) { return Matrix4(cosf(rad), 0.0f, -sinf(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(rad), 0.0f, cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4 RotationZ(float rad) { return Matrix4(cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4 Scaling(float s) { return Matrix4(s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4 Scaling(float sx, float sy, float sz) { return Matrix4(sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4 Scaling(const Vector3& s) { return Matrix4(s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f, s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

		Matrix4 operator-() const
		{
			return Matrix4(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44);
		}
		Matrix4 operator+(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
				_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
				_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
				_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
		}
		Matrix4 operator-(const Matrix4& rhs) const
		{
			return Matrix4(
				_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
				_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
				_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
				_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
		}
		Matrix4 operator*(const Matrix4& rhs) const
		{
			return Matrix4(
				(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
				(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
				(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
				(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

				(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
				(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
				(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
				(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

				(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
				(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
				(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
				(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

				(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
				(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
				(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
				(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44));
		}
		Matrix4 operator*(float s) const
		{
			return Matrix4(
				_11 * s, _12 * s, _13 * s, _14 * s,
				_21 * s, _22 * s, _23 * s, _24 * s,
				_31 * s, _32 * s, _33 * s, _34 * s,
				_41 * s, _42 * s, _43 * s, _44 * s);
		}
		Matrix4 operator/(float s) const
		{
			//ASSERT(s != 0.0f, "Cannot divide by 0!");
			const float inv = 1.0f / s;
			return (*this) * inv;
		}
		Matrix4 operator+=(const Matrix4& rhs)
		{
			_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
			_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
			_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
			_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
			return *this;
		}

		bool operator==(const Matrix4& rhs)
		{
			if (_11 == rhs._11 && _12 == rhs._12 && _13 == rhs._13 && _14 == rhs._14 &&
				_21 == rhs._21 && _22 == rhs._22 && _23 == rhs._23 && _24 == rhs._24 &&
				_31 == rhs._31 && _32 == rhs._32 && _33 == rhs._33 && _34 == rhs._34 &&
				_41 == rhs._41 && _42 == rhs._42 && _43 == rhs._43 && _44 == rhs._44)
			{
				return true;
			}
			return false;
		}
	};

	class Transform
	{
	public:
		Transform()
		{
			matrix = Matrix3::Identity();
			matrix.SetScale(Vector2::One());
		}

		Transform(const Vector2& pos)
		{
			matrix = Matrix3::Identity();
			matrix.SetScale(Vector2::One());
			matrix.SetTranslation(pos);
		}

		Transform(const Vector2& pos, const Rotator& rot, const Vector2& sca)
		{
			matrix = Matrix3::Identity();
			matrix.SetScale(sca);
			matrix.SetRotation(rot.GetRad());
			matrix.SetTranslation(pos);
		}

		Transform(const Transform& transf) { matrix = transf.matrix; }
		Transform(const Matrix3& mat) { matrix = mat; }

		Vector2 GetPosition() const { return matrix.GetTranslation(); }
		Rotator GetRotation() const { Rotator result; result.SetRad(matrix.GetRotation()); return result; }
		Vector2 GetScale() const { return matrix.GetTranslation(); }

		void SetPosition(const Vector2& pos) { matrix.SetTranslation(pos); }
		void SetRotation(const Rotator rot) { matrix.SetRotation(rot.GetRad()); }
		void SetScale(const Vector2& sca) { matrix.SetScale(sca); }

		Transform CombineWithParent(const Transform& parent) const { return Transform(parent.matrix * matrix); }
	
		Matrix3 matrix;
	};

	//----------------------------------------------------------------------------------------------------

	struct LineSegment
	{
		Vector2 from;
		Vector2 to;

		LineSegment() : from(0.0f, 0.0f), to(0.0f, 0.0f) {}
		LineSegment(float x0, float y0, float x1, float y1) : from(x0, y0), to(x1, y1) {}
		LineSegment(const Vector2& v0, const Vector2& v1) : from(v0), to(v1) {}
	};

	//----------------------------------------------------------------------------------------------------

	struct Rect
	{
		float left, top, right, bottom;

		Rect() : left(0.0f), top(0.0f), right(1.0f), bottom(1.0f) {}
		Rect(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}
		operator sf::FloatRect() const { return { left, top, right - left, bottom - top }; };							// **EDIT** Added for SFML Compatibility
		operator sf::IntRect() const { return { (int)left, (int)top, (int)(right - left), (int)(bottom - top) }; };		// **EDIT** Added for SFML Compatibility
	};

	//----------------------------------------------------------------------------------------------------

	struct Circle
	{
		Vector2 center;
		float radius;

		Circle() : center(0.0f, 0.0f), radius(1.0f) {}
		Circle(const Vector2& center, float radius) : center(center), radius(radius) {}
		Circle(float x, float y, float radius) : center(x, y), radius(radius) {}
	};

	//----------------------------------------------------------------------------------------------------

	struct AABB
	{
		Vector3 center;
		Vector3 extend;

		AABB() : center(0.0f, 0.0f, 0.0f), extend(0.0f, 0.0f, 0.0f) {}
		AABB(const Vector3& center, const Vector3& extend) : center(center), extend(extend) {}

	};


	template <typename T> inline T Min(T a, T b) { return (a > b) ? b : a; }
	template <typename T> inline T Max(T a, T b) { return (a < b) ? b : a; }
	//----------------------------------------------------------------------------------------------------
	struct AABB2D
	{
		float Xmin;
		float Ymin;
		float Xmax;
		float Ymax;

		AABB2D() : Xmin(0.0f), Ymin(0.0f), Xmax(0.0f), Ymax(0.0f) {}
		AABB2D(float xmin, float ymin, float xmax, float ymax) : Xmin(xmin), Ymin(ymin), Xmax(xmax), Ymax(ymax) {}

		void CalculateAABB2D(std::vector<Vector3> vertices)
		{
			Xmin = vertices[0].x;;
			Ymin = vertices[0].y;;
			Xmax = vertices[0].x;;
			Ymax = vertices[0].y;;

			for (uint32_t i = 0; i < vertices.size(); ++i)
			{

				Xmin = Min(Xmin, vertices[i].x);
				Ymin = Min(Ymin, vertices[i].y);
				Xmax = Max(Xmax, vertices[i].x);
				Ymax = Max(Ymax, vertices[i].y);
			}
		}

	};

	//----------------------------------------------------------------------------------------------------

	struct Sphere
	{
		Vector3 center;
		float radius;

		Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
		Sphere(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
		Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}
	};

	//----------------------------------------------------------------------------------------------------

	struct Ray
	{
		Vector3 org;
		Vector3 dir;

		Ray() : org(0.0f, 0.0f, 0.0f), dir(0.0f, 0.0f, 0.0f) {}
		Ray(float x, float y, float z, float dx, float dy, float dz) : org(x, y, z), dir(dx, dy, dz) {}
		Ray(const Vector3& org, const Vector3& dir) : org(org), dir(dir) {}
	};

	//----------------------------------------------------------------------------------------------------

	struct Plane
	{
		Vector3 n;
		float d;

		Plane() : n(0.0f, 1.0f, 0.0f), d(0.0f) {}
		Plane(float a, float b, float c, float d) : n(a, b, c), d(d) {}
	};

	namespace Math
	{
		//----------------------------------------------------------------------------------------------------

		inline const float kPi = 3.14159265358979f;
		inline const float kTwoPi = 6.28318530717958f;
		inline const float kPiByTwo = 1.57079632679489f;
		inline const float kRootTwo = 1.41421356237309f;
		inline const float kRootThree = 1.73205080756887f;
		inline const float kDegToRad = kPi / 180.0f;
		inline const float kRadToDeg = 180.0f / kPi;

		//----------------------------------------------------------------------------------------------------


		template <typename T> inline T Clamp(T value, T min, T max) { return Max(min, Min(max, value)); }

		inline float Abs(float value) { return (value >= 0.0f) ? value : -value; }
		inline float Sign(float value) { return (value >= 0.0f) ? 1.0f : -1.0f; }
		inline float Sqr(float value) { return value * value; }
		inline float Sqrt(float value) { return sqrtf(value); }

		inline bool Compare(float a, float b, float epsilon = FLT_MIN) { return Abs(a - b) <= epsilon; }
		inline bool IsZero(float value) { return Compare(value, 0.0f); }
		inline bool IsZero(const Vector2& v) { return IsZero(v.x) && IsZero(v.y); }
		inline bool IsZero(const Vector3& v) { return IsZero(v.x) && IsZero(v.y) && IsZero(v.z); }

		inline Vector2 PerpendicularLH(const Vector2& v) { return Vector2(-v.y, v.x); }
		inline Vector2 PerpendicularRH(const Vector2& v) { return Vector2(v.y, -v.x); }

		inline float MagnitudeSqr(const Vector2& v) { return (v.x * v.x) + (v.y * v.y); }
		inline float MagnitudeSqr(const Vector3& v) { return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }
		inline float Magnitude(const Vector2& v) { return Sqrt(MagnitudeSqr(v)); }
		inline float Magnitude(const Vector3& v) { return Sqrt(MagnitudeSqr(v)); }
		inline float MagnitudeXZSqr(const Vector3& v) { return (v.x * v.x) + (v.z * v.z); }
		inline float MagnitudeXZ(const Vector3& v) { return Sqrt(MagnitudeXZSqr(v)); }
		inline float Magnitude(const Quaternion& q) { return Sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w)); }
		inline float MagnitudeSqr(const Quaternion& q) { return (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w); }


		inline Vector2 Normalize(const Vector2& v) { return v / Magnitude(v); }
		inline Vector3 Normalize(const Vector3& v) { return v / Magnitude(v); }
		inline Quaternion Normalize(const Quaternion& q) { return q / Magnitude(q); }

		inline float DistanceSqr(const Vector2& a, const Vector2& b) { return MagnitudeSqr(a - b); }
		inline float DistanceSqr(const Vector3& a, const Vector3& b) { return MagnitudeSqr(a - b); }
		inline float Distance(const Vector2& a, const Vector2& b) { return Sqrt(DistanceSqr(a, b)); }
		inline float Distance(const Vector3& a, const Vector3& b) { return Sqrt(DistanceSqr(a, b)); }
		inline float DistanceXZSqr(const Vector3& a, const Vector3& b) { return MagnitudeXZSqr(a - b); }
		inline float DistanceXZ(const Vector3& a, const Vector3& b) { return Sqrt(DistanceXZSqr(a, b)); }
		inline float Dot(const Vector2& a, const Vector2& b) { return (a.x * b.x) + (a.y * b.y); }
		inline float Dot(const Vector3& a, const Vector3& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
		inline Vector3 Cross(const Vector3& a, const Vector3& b) { return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)); }
		inline Vector2 Project(const Vector2& v, const Vector2& n) { return n * (Dot(v, n) / Dot(n, n)); }
		inline Vector3 Project(const Vector3& v, const Vector3& n) { return n * (Dot(v, n) / Dot(n, n)); }
		inline Vector2 Reflect(const Vector2& v, const Vector2& normal) { return v - (normal * Dot(v, normal) * 2.0f); }
		inline Vector3 Reflect(const Vector3& v, const Vector3& normal) { return v - (normal * Dot(v, normal) * 2.0f); }

		inline Vector2 GetTranslation(const Matrix3& m) { return Vector2(m._31, m._32); }
		inline Vector3 GetTranslation(const Matrix4& m) { return Vector3(m._41, m._42, m._43); }
		inline Vector3 GetRight(const Matrix4& m) { return Vector3(m._11, m._12, m._13); }
		inline Vector3 GetUp(const Matrix4& m) { return Vector3(m._21, m._22, m._23); }
		inline Vector3 GetForward(const Matrix4& m) { return Vector3(m._31, m._32, m._33); }

		inline Vector3 GetPoint(const Ray& ray, float distance) { return ray.org + (ray.dir * distance); }

		inline Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t) { return v0 + ((v1 - v0) * t); }
		inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t) { return v0 + ((v1 - v0) * t); }
		inline Quaternion Lerp(Quaternion q0, Quaternion q1, float t) { return q0 * (1.0f - t) + (q1 * t); }


		inline Vector2 Rotate(const Vector2& v, float rad)
		{
			const float kCosAngle = cos(rad);
			const float kSinAngle = sin(rad);
			return Vector2
			(
				v.x * kCosAngle - v.y * kSinAngle,
				v.y * kCosAngle + v.x * kSinAngle
			);
		}

		inline float Determinant(const Matrix3& m)
		{
			float det = 0.0f;
			det = (m._11 * (m._22 * m._33 - m._23 * m._32));
			det -= (m._12 * (m._21 * m._33 - m._23 * m._31));
			det += (m._13 * (m._21 * m._32 - m._22 * m._31));
			return det;
		}

		inline float Determinant(const Matrix4& m)
		{
			float det = 0.0f;
			det = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
			det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
			det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
			det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
			return det;
		}

		inline Matrix3 Adjoint(const Matrix3& m)
		{
			return Matrix3
			(
				(m._22 * m._33 - m._23 * m._32),
				-(m._12 * m._33 - m._13 * m._32),
				(m._12 * m._23 - m._13 * m._22),

				-(m._21 * m._33 - m._23 * m._31),
				(m._11 * m._33 - m._13 * m._31),
				-(m._11 * m._23 - m._13 * m._21),

				(m._21 * m._32 - m._22 * m._31),
				-(m._11 * m._32 - m._12 * m._31),
				(m._11 * m._22 - m._12 * m._21)
			);
		}

		inline Matrix4 Adjoint(const Matrix4& m)
		{
			return Matrix4
			(
				(m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
				-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
				(m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
				-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

				-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
				(m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
				-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
				(m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

				(m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
				-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
				(m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
				-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

				-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
				(m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
				-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
				(m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
			);
		}

		inline Matrix3 Inverse(const Matrix3& m)
		{
			const float determinant = Determinant(m);
			const float invDet = 1.0f / determinant;
			return Adjoint(m) * invDet;
		}

		inline Matrix4 Inverse(const Matrix4& m)
		{
			const float determinant = Determinant(m);
			const float invDet = 1.0f / determinant;
			return Adjoint(m) * invDet;
		}

		inline Matrix3 Transpose(const Matrix3& m)
		{
			return Matrix3
			(
				m._11, m._21, m._31,
				m._12, m._22, m._32,
				m._13, m._23, m._33
			);
		}

		inline Matrix4 Transpose(const Matrix4& m)
		{
			return Matrix4
			(
				m._11, m._21, m._31, m._41,
				m._12, m._22, m._32, m._42,
				m._13, m._23, m._33, m._43,
				m._14, m._24, m._34, m._44
			);
		}

		inline Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
		{
			return Vector2
			(
				v.x * m._11 + v.y * m._21 + m._31,
				v.x * m._12 + v.y * m._22 + m._32
			);
		}

		inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
		{
			return Vector3
			(
				v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
				v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
				v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
			);
		}

		inline Vector2 TransformNormal(const Vector2& v, const Matrix3& m)
		{
			return Vector2
			(
				v.x * m._11 + v.y * m._21,
				v.x * m._12 + v.y * m._22
			);
		}

		inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
		{
			return Vector3
			(
				v.x * m._11 + v.y * m._21 + v.z * m._31,
				v.x * m._12 + v.y * m._22 + v.z * m._32,
				v.x * m._13 + v.y * m._23 + v.z * m._33
			);
		}

		/*Quaternion QuaternionLookRotation(const Vector3& forward, const Vector3& up);
		Quaternion QuaternionRotationAxis(const Vector3& axis, float rad);*/
		Quaternion Conjugate(Quaternion q0);
		Quaternion Inverse(Quaternion q0);

		Matrix4 MatrixRotationAxis(const Vector3& axis, float rad);
		Matrix4 QuaternionToMatrix(const Quaternion& q);

		Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
		Quaternion EulerToQuaternion(float x, float y, float z);
		Vector3 QuaternionToEuler(const Quaternion& q);
		Quaternion LookRotation(const Vector3& look, const Vector3& up);
		Quaternion FromToQuaternion(const Vector3& from, const Vector3& to);
		Quaternion QuaternionFromAxisAngle(const Vector3& axis, float angleDegrees);
		Quaternion QuaternionFromMatrix(const Matrix4& rotationMatrix);

		bool PointInRect(const Vector2& point, const Rect& rect);
		bool PointInCircle(const Vector2& point, const Circle& circle);

		bool Intersect(const AABB2D& aabb0, const AABB2D& aabb1);

		bool Intersect(const LineSegment& a, const LineSegment& b);
		bool Intersect(const Circle& c0, const Circle& c1);
		bool Intersect(const Rect& r0, const Rect& r1);

		bool Intersect(const LineSegment& l, const Circle& c);
		bool Intersect(const Circle& c, const LineSegment& l);

		bool Intersect(const Circle& c, const Rect& r);
		bool Intersect(const Rect& r, const Circle& c);

		bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance);
		bool Intersect(const Ray& ray, const Plane& plane, float& distance);
		bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);
		/*bool Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit);*/
		bool Intersect(const Vector3& point, const AABB& aabb);
		/*bool Intersect(const Vector3& point, const OBB& obb);*/

		/*void GetCorners(const OBB& obb, std::vector<Vector3>& corners);
		bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal);*/

		Rect GetAABB2DIntersectionRect(const AABB2D& aabb0, const AABB2D& aabb1);

		Vector3 GetClosestPoint(const Ray& ray, const Vector3& point);

		Vector3 Mean(const Vector3* v, uint32_t count);

		//float RandomFloat(float min, float max);
		//int RandomInt(int min, int max);

		Vector3 VecMax(const Vector3& a, const Vector3& b);

		Vector3 VecMin(const Vector3& a, const Vector3& b);

		float Remap(float oldLower, float oldUpper, float newLower, float newUpper, float value);
		int Round(float num);
		float Round(float num, int decPlace);

		bool FloatComp(float a, float b);

		class Random final
		{
			Random();
			static Random& Get();

		public:
			static int Range(int lower, int upper, float increment = 1);
			static float Range(float lower, float upper, float increment);
			static uint32_t UInt32();
		};
	}//namespace Math
}