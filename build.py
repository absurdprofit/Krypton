import os
import shutil
import uuid

hash = uuid.uuid4().hex

command = 'emcc'
for (dirpath, dirnames, filenames) in os.walk(os.getcwd()):
    for file in filenames:
        if '.cpp' in file:
            command += ' "'
            command += dirpath
            command += '\\'
            command += file
            command += '"'


command += f" --bind -s USE_SDL=2 --no-heap-copy -s ALLOW_MEMORY_GROWTH=1 -s USE_FREETYPE=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s EXIT_RUNTIME=1  --preload-file assets -o app.{hash}.js"
print("Compiling to wasm...")
os.system(f'cmd /c {command}')

js = open(f'app.{hash}.js', "r")
jsSrc = js.read()
js.close()
js = open(f"app.{hash}.js", "w")
js.write(jsSrc.replace(f"var PACKAGE_NAME = 'app.{hash}.data';", f"var PACKAGE_NAME = '../static/assets/app.{hash}.data';").replace(f"var REMOTE_PACKAGE_BASE = 'app.{hash}.data';",
                                                                                                                    f"var REMOTE_PACKAGE_BASE = '../static/assets/app.{hash}.data';").replace(f"var wasmBinaryFile = 'app.{hash}.wasm';", f"var wasmBinaryFile = '../wasm/app.{hash}.wasm';"))
js.close()
filelocs = ['../templates/', '../static/css/', '../static/js/', '../static/assets/', '../static/wasm/']

for fileloc in filelocs:
    if os.path.exists(fileloc):
        shutil.rmtree(fileloc)
        os.mkdir(fileloc)

shutil.move(f'app.{hash}.js', '../static/js')
shutil.move(f'app.{hash}.wasm', '../static/wasm')
shutil.move(f'app.{hash}.data', '../static/assets')

def moveTo(src, dst):
	f = open(src, "r")
	fStr = f.read()
	f.close()
	f = open(dst, "w+")
	f.write(fStr)
	f.close()

moveTo('common/styles.css', f'../static/css/styles.{hash}.css')
moveTo('common/driver.js', f'../static/js/driver.{hash}.js')
moveTo('common/main.html', f'../templates/main.{hash}.html')