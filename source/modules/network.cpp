#include <pymodule.h>

#include <MOHPC/Managers/NetworkManager.h>
#include <MOHPC/Network/MasterList.h>

namespace m_ = MOHPC;
namespace mnet = MOHPC::Network;

namespace mohpcpy
{
	class ServerList
	{
	public:
		static void fetch(mnet::ServerList* This, py::function found, py::function done)
		{
			if (!found) {
				throw std::invalid_argument("found");
			}

			if (!done) {
				throw std::invalid_argument("done");
			}

			This->fetch(found, done);
		}
	};

	class IServer
	{
	public:
		static void query(mnet::IServer* This, py::function response, py::function timeout)
		{
			if (!response) {
				throw std::invalid_argument("response");
			}

			if (!timeout) {
				throw std::invalid_argument("timeout");
			}

			This->query(response, timeout);
		}
	};
}

SUBMODULE(network, m)
{
	m.doc() = "MOHPC Network module";

	py::class_<m_::NetworkManager>(m, "NetworkManager", "Network Manager")
		.def("tick", &MOHPC::NetworkManager::processTicks);

	py::enum_<mnet::gameListType_e>(m, "GameListType", "Valid list that can be queried")
		.value("mohaa", mnet::gameListType_e::mohaa, "Medal of Honor Allied Assault (1.0-1.11) server list")
		.value("mohaas", mnet::gameListType_e::mohaas, "Medal of Honor Allied Assault: Spearhead (2.0-2.15) server list")
		.value("mohaab", mnet::gameListType_e::mohaab, "Medal of Honor Allied Assault: Breakthrough (2.30-2.40) server list");

	py::class_<mnet::ServerList, mnet::ServerListPtr>(m, "ServerList", "Server list browser")
		.def(py::init(&mnet::ServerList::CreatePtr<m_::NetworkManager*, mnet::gameListType_e&>), py::arg("network_manager"), py::arg("glist_type"))
		.def("fetch", &mohpcpy::ServerList::fetch, py::arg("found"), py::arg("done"), "Query and fetch the server list");

	py::class_<mnet::IServer, mnet::IServerPtr>(m, "Server", "Represents a server")
		.def_property_readonly("address", &mnet::IServer::getAddress, py::return_value_policy::reference)
		.def("query", &mohpcpy::IServer::query, py::arg("response"), py::arg("timeout"), py::return_value_policy::reference);

	py::class_<mnet::netadr_t>(m, "NetAddress", "Network address")
		.def("__str__", [](const mnet::netadr_t& addr) -> std::string { return std::string(std::to_string(addr.ip[0]) + "." + std::to_string(addr.ip[1]) + "." + std::to_string(addr.ip[2]) + "." + std::to_string(addr.ip[3])); });
}
