#include <sys/stat.h>
#include <fstream>
#include <statichook.h>

std::string capture = "default_world";
auto textVT = (size_t) dlsym(MinecraftHandle(), "_ZTV10TextPacket") + sizeof(size_t) * 2;

inline void createSubFolder() {
	char folder[256];
	sprintf(folder, "chunks/%s", capture.c_str());

	mkdir(folder, 0755);
}

inline void createFile(char const *filename, std::string const &buffer) {
	std::ofstream output(filename);
	output << buffer;
	output.close();
}

inline bool fileNotExist(char *filename) {
	struct stat buffer{};
	return stat(filename, &buffer) != 0;
}

extern "C" {
void *mcpelauncher_hook(void *symbol, void *hook, void **original) { return nullptr; }

void mod_init() {
	mkdir("chunks", 0755);
	createSubFolder();
}
}

struct Packet {
	size_t vt;
	char filler[20 - sizeof(vt)];
};

struct TextPacket : Packet {
	unsigned char type;
	std::string sender, message;
};

TClasslessInstanceHook(void, _ZN20LoopbackPacketSender4sendER6Packet, Packet &packet) {
	if (packet.vt == textVT) {
		auto pk = (TextPacket *) &packet;
		auto message = pk->message;
		if (message.find("capture") != std::string::npos && message.size() > 8) {
			capture = message.substr(8);
			createSubFolder();
			return; //cancel packet
		}
	}
	original(this, packet);
}

struct BinaryStream {
	char filler[sizeof(size_t) * 4];
	std::string buffer;
};

struct TransferPacket : Packet {
	std::string server;
	unsigned short port;
};

TInstanceHook(void *, _ZN14TransferPacket4readER12BinaryStream, TransferPacket, BinaryStream &stream) {
	auto ret = original(this, stream);
	char filename[256];
	sprintf(filename, "world_%s_%hu", server.c_str(), port);
	capture = filename;
	return ret;
}

TClasslessInstanceHook(void *, _ZN15StartGamePacket4readER12BinaryStream, BinaryStream &stream) {
	auto ret = original(this, stream);
	createFile("chunks/.table", stream.buffer);
	return ret;
}

struct FullChunkDataPacket : Packet {
	int chunkX, chunkZ;
};

TInstanceHook(void *, _ZN19FullChunkDataPacket4readER12BinaryStream, FullChunkDataPacket, BinaryStream &stream) {
	auto ret = original(this, stream);

	char filename[256];
	sprintf(filename, "chunks/%s/%d_%d", capture.c_str(), chunkX, chunkZ);

	if (fileNotExist(filename))
		createFile(filename, stream.buffer);

	return ret;
}