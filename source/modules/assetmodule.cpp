#include <pymodule.h>
#include <templates/iteration.h>

#include <MOHPC/Managers/AssetManager.h>
#include <MOHPC/Managers/NetworkManager.h>
#include <MOHPC/Formats/BSP.h>
#include <MOHPC/LevelEntity.h>

#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include <vector>
#include <string>
#include <future>

namespace mohpcpy
{
	class BSP
	{
	public:
		DECLARE_ITERATOR(ShaderIterator,			MOHPC::BSP&, const MOHPC::BSP::Shader&, GetNumShaders, GetShader);
		DECLARE_ITERATOR(LightmapIterator,			MOHPC::BSP&, const MOHPC::BSP::Lightmap&, GetNumLightmaps, GetLightmap);
		DECLARE_ITERATOR(SurfaceIterator,			MOHPC::BSP&, const MOHPC::BSP::Surface&, GetNumSurfaces, GetSurface);
		DECLARE_ITERATOR(PlaneIterator,				MOHPC::BSP&, const MOHPC::BSP::Plane&, GetNumPlanes, GetPlane);
		DECLARE_ITERATOR(SideEquationIterator,		MOHPC::BSP&, const MOHPC::BSP::SideEquation&, GetNumSideEquations, GetSideEquation);
		DECLARE_ITERATOR(BrushSideIterator,			MOHPC::BSP&, const MOHPC::BSP::BrushSide&, GetNumBrushSides, GetBrushSide);
		DECLARE_ITERATOR(SubmodelIterator,			MOHPC::BSP&, const MOHPC::BSP::Model&, GetNumSubmodels, GetSubmodel);
		DECLARE_ITERATOR(NodeIterator,				MOHPC::BSP&, const MOHPC::BSP::Node&, GetNumNodes, GetNode);
		DECLARE_ITERATOR(LeafBrushIterator,			MOHPC::BSP&, uintptr_t, GetNumLeafBrushes, GetLeafBrush);
		DECLARE_ITERATOR(LeafSurfaceIterator,		MOHPC::BSP&, uintptr_t, GetNumLeafSurfaces, GetLeafSurface);
		DECLARE_ITERATOR(LightIterator,				MOHPC::BSP&, const MOHPC::BSP::SphereLight&, GetNumLights, GetLight);
		DECLARE_ITERATOR(StaticModelIterator,		MOHPC::BSP&, const MOHPC::BSP::StaticModel&, GetNumStaticModels, GetStaticModel);
		DECLARE_ITERATOR(TerrainPatchIterator,		MOHPC::BSP&, const MOHPC::BSP::TerrainPatch&, GetNumTerrainPatches, GetTerrainPatch);
		DECLARE_ITERATOR(TerrainSurfaceIterator,	MOHPC::BSP&, const MOHPC::BSP::Surface&, GetNumTerrainSurfaces, GetTerrainSurface);
		DECLARE_ITERATOR(EntityIterator,			MOHPC::BSP&, const MOHPC::LevelEntity&, GetNumEntities, GetEntity);
		DECLARE_ITERATOR(SurfacesGroupIterator,		MOHPC::BSP&, const MOHPC::BSP::SurfacesGroup&, GetNumSurfacesGroup, GetSurfacesGroup);

	public:
		DECLARE_GETTER(get_shaders, ShaderIterator, MOHPC::BSP);
		DECLARE_GETTER(get_lightmaps, LightmapIterator, MOHPC::BSP);
		DECLARE_GETTER(get_surfaces, SurfaceIterator, MOHPC::BSP);
		DECLARE_GETTER(get_planes, PlaneIterator, MOHPC::BSP);
		DECLARE_GETTER(get_sideequations, SideEquationIterator, MOHPC::BSP);
		DECLARE_GETTER(get_brushsides, BrushSideIterator, MOHPC::BSP);
		DECLARE_GETTER(get_submodels, SubmodelIterator, MOHPC::BSP);
		DECLARE_GETTER(get_nodes, NodeIterator, MOHPC::BSP);
		DECLARE_GETTER(get_leafbrushes, LeafBrushIterator, MOHPC::BSP);
		DECLARE_GETTER(get_leafsurfaces, LeafSurfaceIterator, MOHPC::BSP);
		DECLARE_GETTER(get_lights, LightIterator, MOHPC::BSP);
		DECLARE_GETTER(get_staticmodels, StaticModelIterator, MOHPC::BSP);
		DECLARE_GETTER(get_terrainpatches, TerrainPatchIterator, MOHPC::BSP);
		DECLARE_GETTER(get_terrainsurfaces, TerrainSurfaceIterator, MOHPC::BSP);
		DECLARE_GETTER(get_entities, EntityIterator, MOHPC::BSP);
		DECLARE_GETTER(get_surfacesgroups, SurfacesGroupIterator, MOHPC::BSP);
	};

	DECLARE_ITERATOR(FileEntryIterator, MOHPC::FileEntryList, const MOHPC::FileEntry&, GetNumFiles, GetFileEntry);

	class FileManager
	{
	public:
		static mohpcpy::FileEntryIterator list_filtered_files(MOHPC::FileManager* fm, const char* dir, const std::vector<std::string>& extensions, bool recurse = true, bool only_pak = true, const char* category_name = nullptr)
		{
			MOHPC::Container<MOHPC::str> ext_wrap;
			ext_wrap.Resize(extensions.size());
			for (const std::string& ext : extensions)
			{
				ext_wrap.AddObject(ext.c_str());
			}

			return mohpcpy::FileEntryIterator(
				fm->ListFilteredFiles(dir, ext_wrap, recurse, only_pak, category_name)
			);
		}
	};
}

class Test {};

SUBMODULE(asset_module, m)
{
	m.doc() = "Module for assets manipulation";

	py::class_<MOHPC::AssetManager>(m, "AssetManager", "Class for loading assets")
		.def(py::init<>())
		.def("load_bsp", &MOHPC::AssetManager::LoadAsset<MOHPC::BSP>, "Load BSP asset", py::return_value_policy::reference_internal)
		.def("get_file_manager", &MOHPC::AssetManager::GetFileManager, "Get the virtual file manager", py::return_value_policy::reference_internal)
		.def("get_network_manager", &MOHPC::AssetManager::GetManager<MOHPC::NetworkManager>, py::return_value_policy::reference_internal);

	py::class_<MOHPC::FileManager>(m, "FileManager", "Virtual file manager")
		.def("fill_game_dir", &MOHPC::FileManager::FillGameDirectory, py::arg("game_dir"), "Automatically populate the file manager with pak files")
		.def("list_filtered_files", &mohpcpy::FileManager::list_filtered_files, "List files", py::return_value_policy::move,
		py::arg("dir"), py::arg("extensions"), py::arg("recurse") = true, py::arg("only_pak") = true, py::arg("category_name") = nullptr);

	py::class_<MOHPC::FileEntry>(m, "FileEntry", "File entry")
		.def_property_readonly("name", &MOHPC::FileEntry::GetRawName, "Entry name (file/directory name)")
		.def_property_readonly("is_dir", &MOHPC::FileEntry::IsDirectory, "Whether or not the entry is a directory")
		.def_property_readonly("extension", &MOHPC::FileEntry::GetExtension, "File extension");

	mohpcpy::declare_iterator<mohpcpy::FileEntryIterator>(m, "FileEntryList", "File list");

	//== BSP class
	py::class_<MOHPC::BSP, MOHPC::BSPPtr> class_bsp(m, "BSPAsset", "MOH:AA BSP Level asset");
		class_bsp.def_property_readonly("shaders", &mohpcpy::BSP::get_shaders, "List of shaders")
		.def_property_readonly("lightmaps", &mohpcpy::BSP::get_lightmaps, "List of lightmaps")
		.def_property_readonly("surfaces", &mohpcpy::BSP::get_surfaces, "Drawable surfaces")
		.def_property_readonly("planes", &mohpcpy::BSP::get_planes, "Collision planes")
		.def_property_readonly("sideequations", &mohpcpy::BSP::get_sideequations, "Side equations (for fences)")
		.def_property_readonly("brushsides", &mohpcpy::BSP::get_brushsides, "Brush sides")
		.def_property_readonly("submodels", &mohpcpy::BSP::get_submodels, "Submodels (brushmodel that are assigned to entities)")
		.def_property_readonly("nodes", &mohpcpy::BSP::get_nodes, "BSP Nodes")
		.def_property_readonly("leafbrushes", &mohpcpy::BSP::get_leafbrushes, "Leaf brushes for collision check")
		.def_property_readonly("leafsurfaces", &mohpcpy::BSP::get_leafsurfaces, "Leaf surfaces for collision check")
		.def_property_readonly("lights", &mohpcpy::BSP::get_lights, "Compiled lights")
		.def_property_readonly("staticmodels", &mohpcpy::BSP::get_staticmodels, "List of models that are static (and don't count as entities)")
		.def_property_readonly("terrainpatches", &mohpcpy::BSP::get_terrainpatches, "List of terrain patches")
		.def_property_readonly("terrainsurfaces", &mohpcpy::BSP::get_terrainsurfaces, "List of terrain surfaces")
		.def_property_readonly("entities", &mohpcpy::BSP::get_entities, "List of entities in the level")
		.def_property_readonly("surfacesgroups", &mohpcpy::BSP::get_surfacesgroups, "List of grouped surfaces for actor count optimization");
		
	//=== BSP iterators
	mohpcpy::declare_iterator<mohpcpy::BSP::ShaderIterator>(class_bsp, "ShaderIterator", "test");
	mohpcpy::declare_iterator<mohpcpy::BSP::LightmapIterator>(class_bsp, "LightmapIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::SurfaceIterator>(class_bsp, "SurfaceIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::PlaneIterator>(class_bsp, "PlaneIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::SideEquationIterator>(class_bsp, "SideEquationIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::BrushSideIterator>(class_bsp, "BrushSideIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::SubmodelIterator>(class_bsp, "SubmodelIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::NodeIterator>(class_bsp, "NodeIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::LeafBrushIterator>(class_bsp, "LeafBrushIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::LeafSurfaceIterator>(class_bsp, "LeafSurfaceIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::LightIterator>(class_bsp, "LightIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::StaticModelIterator>(class_bsp, "StaticModelIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::TerrainPatchIterator>(class_bsp, "TerrainPatchIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::TerrainSurfaceIterator>(class_bsp, "TerrainSurfaceIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::EntityIterator>(class_bsp, "EntityIterator");
	mohpcpy::declare_iterator<mohpcpy::BSP::SurfacesGroupIterator>(class_bsp, "SurfacesGroupIterator");

	//=== BSP structs
	{
		using Shader = MOHPC::BSP::Shader;

		py::class_<Shader>(class_bsp, "Shader", "Shader from BSP")
			.def_readonly("name", &Shader::shaderName)
			.def_readonly("surface_flags", &Shader::surfaceFlags)
			.def_readonly("content_flags", &Shader::contentFlags)
			.def_readonly("subdivisions", &Shader::subdivisions)
			.def_readonly("fence_mask_image", &Shader::fenceMaskImage)
			.def_readonly("shader", &Shader::shader);
	}
	{
		using Plane = MOHPC::BSP::Plane;

		py::class_<Plane>(class_bsp, "Plane", "BSP planes")
			.def_readonly("normal", &Plane::normal)
			.def_readonly("distance", &Plane::distance)
			.def_readonly("type", &Plane::type)
			.def_readonly("sign_bits", &Plane::signBits);
	}
	{
		using PatchPlane = MOHPC::BSP::PatchPlane;

		py::class_<PatchPlane>(class_bsp, "PatchPlane", "BSP patch plane")
			.def_readonly("plane", &PatchPlane::plane)
			.def_readonly("sign_bits", &PatchPlane::signbits);
	}
	{
		using Facet = MOHPC::BSP::Facet;

		py::class_<Facet>(class_bsp, "Facet", "BSP patch facet")
			.def_readonly("surface_plane", &Facet::surfacePlane)
			.def_readonly("num_borders", &Facet::numBorders)
			.def_readonly("border_planes", &Facet::borderPlanes)
			.def_readonly("border_inward", &Facet::borderInward)
			.def_readonly("border_no_adjust", &Facet::borderNoAdjust);
	}
	{
		using PatchCollide = MOHPC::BSP::PatchCollide;

		py::class_<PatchCollide>(class_bsp, "PatchCollide", "BSP patch collision")
			.def_property_readonly("mins", [](const PatchCollide& This) { return This.bounds[0]; })
			.def_property_readonly("maxs", [](const PatchCollide& This){ return This.bounds[1]; })
			.def_readonly("num_planes", &PatchCollide::numPlanes)
			.def_readonly("planes", &PatchCollide::planes)
			.def_readonly("num_facets", &PatchCollide::numFacets)
			.def_readonly("facets", &PatchCollide::facets);
	}

	py::class_<MOHPC::LevelEntity>(m, "LevelEntity", "Level entity")
		.def_property_readonly("classname", &MOHPC::LevelEntity::GetClassName)
		.def_property_readonly("entnum", &MOHPC::LevelEntity::GetEntNum)
		.def_property_readonly("spawnflags", &MOHPC::LevelEntity::GetSpawnflags)
		.def_property_readonly("targetname", &MOHPC::LevelEntity::GetTargetName)
		.def_property_readonly("target", &MOHPC::LevelEntity::GetTarget)
		.def_property_readonly("model", &MOHPC::LevelEntity::GetModel)
		.def_property_readonly("angles", &MOHPC::LevelEntity::GetAngles)
		.def_property_readonly("origin", &MOHPC::LevelEntity::GetOrigin)
		.def_property_readonly("properties", &MOHPC::LevelEntity::GetPropertyObject);
}
