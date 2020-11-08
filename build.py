import os
import shutil

command = 'emcc'
for (dirpath, dirnames, filenames) in os.walk(os.getcwd()):
    for file in filenames:
        if '.cpp' in file:
            command += ' "'
            command += dirpath
            command += '\\'
            command += file
            command += '"'


command += " --bind -s USE_SDL=2 --no-heap-copy -s ALLOW_MEMORY_GROWTH=1 -s USE_FREETYPE=1 -s MIN_WEBGL_VERSION=1 -s MAX_WEBGL_VERSION=1 -s EXIT_RUNTIME=1  --preload-file assets -o app.js"
os.system(f'cmd /c {command}')

js = open('app.js', "r")
jsSrc = js.read()
js.close()
js = open("app.js", "w")
js.write(jsSrc.replace("var PACKAGE_NAME = 'app.data';", "var PACKAGE_NAME = '../static/assets/app.data';").replace("var REMOTE_PACKAGE_BASE = 'app.data';",
                                                                                                                    "var REMOTE_PACKAGE_BASE = '../static/assets/app.data';").replace("var wasmBinaryFile = 'app.wasm';", "var wasmBinaryFile = '../wasm/app.wasm';"))
js.close()
filelocs = ['../templates/main.html', '../static/css/styles.css', '../static/js/driver.js',
            '../static/js/app.js', '../static/assets/app.data', '../static/wasm/app.wasm']

for fileloc in filelocs:
    if os.path.exists(fileloc):
        os.remove(fileloc)

shutil.move('app.js', '../static/js')
shutil.move('app.wasm', '../static/wasm')
shutil.move('app.data', '../static/assets')
shutil.copy('common/styles.css', '../static/css')
shutil.copy('common/driver.js', '../static/js')
shutil.copy('common/main.html', '../templates')
