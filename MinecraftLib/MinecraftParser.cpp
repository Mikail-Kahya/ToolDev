#include "MinecraftParser.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <regex>
#include <iostream>

MinecraftParser::MinecraftParser(float cubeSize)
	: m_CubeSize{ cubeSize }
{
}

void MinecraftParser::SaveObj(std::string outputName) const
{
	AddExtension(outputName, ".obj");

	std::ofstream outputStream{ (outputName.c_str()) };
	if (!outputStream)
	{
		std::cerr << "Failed to create " << outputName << "\n";
		return;
	}

	// send cubes
	int offset{};
	for (const auto& layer : m_Layers)
	{
		outputStream << "# layer " << layer.second.material.name << "\n";
		outputStream << "# opaque = " << layer.second.material.isOpaque << "\n";

		for (const Cube& cube : layer.second.cubes)
		{
			constexpr int nrVertices{ 8 };
			outputStream << "o Block" << offset / nrVertices << "\n";

			for (const Vector3& vertex : cube.vertices)
				outputStream << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";

			for (const std::vector<int>& face : cube.faces)
				outputStream << "f " << face[0] + offset << " " << face[1] + offset << " " << face[2] + offset << " " << face[3] + offset << "\n";
			offset += nrVertices;
		}
	}


	outputStream.close();
}

void MinecraftParser::SaveJson(std::string outputName) const
{
	AddExtension(outputName, ".json");

	std::ofstream file { outputName };
	if (!file)
	{
		std::cerr << "Failed to create " << outputName << "\n";
		return;
	}


	nlohmann::json data{};

	// The first point is the origin
	int layerIdx{};
	for (const auto& layer : m_Layers)
	{
		nlohmann::json layerData{};
		layerData["opaque"] = layer.second.material.isOpaque;
		layerData["layer"] = layer.second.material.name;

		nlohmann::json positions{};
		for (int idx{}; idx < static_cast<int>(layer.second.cubes.size()); ++idx)
		{
			const Vector3& location{ layer.second.cubes[idx].vertices[0] };
			nlohmann::json position{};
			position[0] = location.x;
			position[2] = location.y;
			position[1] = location.z;
			positions[idx] = position;
		}

		layerData["positions"] = positions;
		data[layerIdx] = layerData;
		++layerIdx;
	}

	file << data;
	file.close();
}

void MinecraftParser::AddCube(const Vector3& position, MaterialType material)
{
	if (material == MaterialType::none)
		return;
	m_Layers[material].cubes.emplace_back(GenerateCube(position));
}

void MinecraftParser::AddCube(const Vector3& position, const std::string& material)
{
	AddCube(position, GetMaterialType(material));
}

void MinecraftParser::AddExtension(std::string& fileName, const std::string& extension) const
{
	const std::regex extensionRegex{ '.' + extension + '$' };
	if (!std::regex_search(fileName, extensionRegex))
		fileName += extension;
}

void MinecraftParser::ParseJson(const std::string& inputName)
{
	std::ifstream inputFile{ inputName.c_str() };
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open input " << inputName << std::endl;
		return;
	}

	nlohmann::json data{};

	try 
	{
		inputFile >> data;
	}
	catch (const nlohmann::json::parse_error& e) 
	{
		std::cerr << "JSON Parsing error: " << e.what() << std::endl;
		return;
	}

	for (const auto& layer : data)
	{
		Layer newLayer{};
		newLayer.material.isOpaque = layer["opaque"];
		newLayer.material.name = layer["layer"];
		for (const auto& jsonPos : layer["positions"])
		{
			Vector3 pos{ jsonPos[0], jsonPos[2], jsonPos[1] };
			newLayer.cubes.push_back(GenerateCube(pos));
		}
		
		m_Layers[GetMaterialType(newLayer.material.name)] = newLayer;
	}
}

MinecraftParser::Cube MinecraftParser::GenerateCube(Vector3 position) const
{
	Cube cube{};

	// Add edges
	Vector3 topPosition{ position.x, position.y + m_CubeSize, position.z };
	// Add vertical edge 1
	cube.vertices.push_back(position);
	cube.vertices.push_back(topPosition);

	// Add vertical edge 2
	position.x += m_CubeSize;
	topPosition.x += m_CubeSize;

	cube.vertices.push_back(topPosition);
	cube.vertices.push_back(position);

	// Add vertical edge 3
	position.z += m_CubeSize;
	topPosition.z += m_CubeSize;

	cube.vertices.push_back(position);
	cube.vertices.push_back(topPosition);

	// Add vertical edge 4
	position.x -= m_CubeSize;
	topPosition.x -= m_CubeSize;

	cube.vertices.push_back(topPosition);
	cube.vertices.push_back(position);

	// Add faces
	constexpr int nrFaces{ 4 };
	constexpr int pointsPerEdge{ 2 };
	constexpr int pointsPerFace{ 4 };
	constexpr int nrVertices{ 8 };

	// Add side faces
	for (int faceIdx{}; faceIdx < nrFaces; ++faceIdx)
	{
		std::vector<int> face(pointsPerFace);
		std::iota(face.begin(), face.end(), (pointsPerEdge * faceIdx) + 1);
		for (int& index : face)
			index = (index > nrVertices) ? index % nrVertices : index;
		cube.faces.push_back(std::move(face));
	}

	// Add top and bottom face
	cube.faces.push_back({ 1, 4, 5, 8 });
	cube.faces.push_back({ 2, 3, 6, 7 });

	return cube;
}

MinecraftParser::Material MinecraftParser::GetMaterial(MaterialType materialType) const
{
	Material material{};

	switch (materialType)
	{
	case MaterialType::dirt:
		material.name = "Dirt";
		material.isOpaque = true;
		break;
	case MaterialType::stone:
		material.name = "Stone";
		material.isOpaque = true;
		break;
	case MaterialType::glass:
		material.name = "Glass";
		material.isOpaque = false;
		break;
	case MaterialType::none:
	default:
		throw MaterialNotFoundException("No material in minecraft found!");
	}

	return material;
}

MaterialType MinecraftParser::GetMaterialType(const std::string& material) const
{
	if (material == "Dirt")
		return MaterialType::dirt;

	if (material == "Stone")
		return MaterialType::stone;

	if (material == "Glass")
		return MaterialType::stone;

	return MaterialType::none;
}
