#pragma once

#include <map>
#include <string>
#include <vector>

struct Vector3
{
	float x{};
	float y{};
	float z{};
};

enum class MaterialType
{
	dirt,
	stone,
	glass,
	none
};

class MaterialNotFoundException
{
public:
	MaterialNotFoundException(std::string&& message) : m_Message{std::move(message) } {}

	std::string GetMessage() const { return "Material could not be found: " + m_Message; }

private:
	std::string m_Message{};
};

class MinecraftParser 
{
private:
	struct Material
	{
		std::string name{ "Dirt" };
		bool isOpaque{ true };
	};

	struct Cube
	{
		std::vector<Vector3> vertices{};
		std::vector<std::vector<int>> faces{};
	};

	struct Layer
	{
		std::vector<Cube> cubes{};
		Material material{};
	};

public:
	MinecraftParser(float cubeSize = 1.f);
	~MinecraftParser()												= default;

	MinecraftParser(const MinecraftParser& other)					= default;
	MinecraftParser(MinecraftParser&& other) noexcept				= default;
	MinecraftParser& operator=(const MinecraftParser& other)		= default;
	MinecraftParser& operator=(MinecraftParser&& other) noexcept	= default;

	void SaveObj(std::string outputName) const;
	void SaveJson(std::string outputName) const;
	void ParseJson(const std::string& inputName);
	void AddCube(const Vector3& position, MaterialType material);
	void AddCube(const Vector3& position, const std::string& material);

private:
	void AddExtension(std::string& fileName, const std::string& extension) const;

	Cube GenerateCube(Vector3 position) const;
	Material GetMaterial(MaterialType materialType) const;
	MaterialType GetMaterialType(const std::string& material) const;

	std::map<MaterialType, Layer> m_Layers{};
	float m_CubeSize{ 1.f };
};