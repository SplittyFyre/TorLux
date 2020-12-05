import glob
import os.path

print('Making make...')

cco = '\tgcc -std=c11 -pedantic -pthread -MMD -Wall -c -I"include" -I"headers" {} -o {}\n'
ccl = '\tgcc -std=c11 -pedantic -pthread -MMD -Wall -L"lib" {} -o {} -lncurses\n'

def getCCO(source, objPath):
    return cco.format(source, objPath)

def getCCL(objs, binout):
    return ccl.format(objs, binout)

def getObjectPath(sourceFile: str):
    return 'obj/' + os.path.basename(sourceFile).replace('.c', '.o')

def getDepPath(sourceFile: str):
    return 'obj/' + os.path.basename(sourceFile).replace('.c', '.d')


lines = []




sources = glob.glob('src/**/*.c', recursive=True)

objs = []

for s in sources:
    objectPath = getObjectPath(s)
    objs.append(objectPath)
    depPath = getDepPath(s)
    if os.path.exists(depPath):
        with open(depPath, 'r') as fin:
            crap = fin.read()
            lines.append('{}\n'.format(crap))
    else:
        lines.append('{}: {}\n'.format(objectPath, s))
    lines.append(getCCO(s, objectPath))

objs = ' '.join(objs)


binary = 'bin/torlux'
lines.append('{}: {}\n'.format(binary, objs))
lines.append(getCCL(objs, binary))

all_line = 'all: {}\n'.format(binary)

clean = '''
.PHONY : clean
clean :
\trm -f obj/*
\trm -f bin/*
'''

with open('Makefile', 'w') as fout:
    fout.write(all_line)
    fout.writelines(lines)

    fout.write(clean)



print('Make made.')
