# WorldDownloader
It's mod for [minecraft-linux/mcpelauncher-client](https://github.com/minecraft-linux/mcpelauncher-client) which collects chunks sent over network from server

### Install
```bash
mkdir mods && cd mods
git clone git@github.com:Frago9876543210/WorldDownloader.git
git clone git@github.com:MCMrARM/mod-ndk-gnustl.git

# Also you should download lastest Android NDK to this folder

cd WorldDownloader

# Now set up all variables in scripts/install.sh (game version, mcpelauncher folder, NDK folder)

scripts/install.sh
```

### Usage
This mod creates folder `~/chunks`
- When player join to server it also collect StartGamePacket into `~/chunk/.table`, which is necessary to obtain information about the paletted chunks
- When a player gets `FullChunkDataPacket` mod store this into `chunks/<capture_name>/chunkX_chunkZ`, `capture_name = default_world` as default
- You can change `capture_name` in runtime. Just send to chunk `capture <new_capture_name>` (message will not be sent)
