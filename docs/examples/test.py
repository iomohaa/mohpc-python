### import the MOHPC Python library and all its submodules
import MOHPCPy
import time

### instanciate an asset manager
print('creating AssetManager')
am = MOHPCPy.asset_module.AssetManager()

def test_network():
    ### instanciate a Network manager
    print("creating NetworkManager")
    netman = am.get_network_manager()

    ### create a server list
    serverList = MOHPCPy.network.ServerList(netman, MOHPCPy.network.GameListType.mohaa)

    ### callbacks
    def server_fetch(server):
        def query(info):
            print('server: ' + str(server.address))
        def timedout():
            print('timeout server ' + str(server.address))
        server.query(query, timedout)
    
    def server_done():
        print('done')

    ### Fetch the server list
    serverList.fetch(server_fetch, server_done)

    ### Loop (to be honest this is not a good way, looking for another ideal way)
    while True:
        netman.tick()
        time.sleep(0.05)


def test_bsp():
    ### get the virtual file manager
    print('getting FileManager')
    fm = am.get_file_manager()
    print('ok: ' + str(fm))

    ### load all pak files from main, mainta and maintt
    fm.fill_game_dir('I:\\Jeux\\Mohaa')

    ### gather all BSP files in the map folder
    files = fm.list_filtered_files('/maps', ['bsp'], True, False)

    ### print them
    for file in files:
        print('Map: ' + file.name)

    ### load the level asset
    bsp = am.load_bsp('/maps/dm/mohdm6.bsp')
    
    ### print all entities properties
    for entity in bsp.entities:
        print('{')

        ### print predefined properties (from SimpleEntity)
        print('// entity ' + str(entity.entnum))
        print(f'"classname" "{entity.classname}"')
        print(f'"spawnflags" "{entity.spawnflags}"')
        if entity.targetname: print(f'"targetname" "{entity.targetname}"')
        if entity.target: print(f'"target" "{entity.target}"')
        if entity.model: print(f'"model" "{entity.model}"')
        print(f'"angles" "{entity.angles}"')
        print(f'"origin" "{entity.origin}"')

        ### print others (can be custom) properties
        if entity.properties:
            print('// miscellaneous properties')
            for prop in entity.properties:
                print(f'"{prop.key.name}" "{prop.value}"')
    
        print('}')

test_bsp()
test_network()
