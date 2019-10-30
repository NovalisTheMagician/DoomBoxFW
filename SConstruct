import os

LD_FILE = 'layout.ld'
CFLAGS = '-mcpu=cortex-m4 -march=armv7e-m -mthumb -Wall -mfpu=fpv4-sp-d16 -mfloat-abi=hard -MMD'
LDFLAGS = '--specs=nano.specs -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -T ' + LD_FILE

hex_bld = Builder(action = 'arm-none-eabi-objcopy -O ihex $SOURCE $TARGET',
                suffix = '.hex',
                src_suffix = '.elf')

env = Environment(  ENV = os.environ,
                    CC = 'arm-none-eabi-gcc',
                    LINK = 'arm-none-eabi-gcc',
                    AR = 'arm-none-eabi-ar',
                    RANLIB = 'arm-none-eabi-ranlib',
                    AS = 'arm-none-eabi-as',
                    tools = ['mingw'],
                    PROGSUFFIX = '.elf',
                    BUILDERS = { 'Hex' : hex_bld })

fw_files = Glob('fw/*.c')
fw_files = fw_files + Glob('fw/hal/*.c')
fw_files = fw_files + Glob('fw/term/*.c')

fat_files = Glob('fat/*.c')
crt_files = Glob('crt0/*.c')

lua_files = Glob('lua/*.c')
app_files = Glob('app/*.c')

fat_objs = env.Object(fat_files, CCFLAGS=CFLAGS + ' -fPIC', CPPPATH=['fat'])
fat = env.Library('lib/fat', fat_objs, LINKFLAGS=LDFLAGS)

crt_objs = env.Object(crt_files, CCFLAGS=CFLAGS + ' -fPIC', CPPPATH=['crt0', '.'])
crt = env.Library('lib/crt0', crt_objs, LIBS=['fat'], LIBPATH=['lib'], LINKFLAGS=LDFLAGS)

lua_objs = env.Object(lua_files, CCFLAGS=CFLAGS + ' -fPIC', CPPPATH=['lua'])
lua = env.Library('lib/lua', lua_objs, LIBS=['m'], LIBPATH=[], LINKFLAGS=LDFLAGS)

app_objs = env.Object(app_files, CCFLAGS=CFLAGS, CPPPATH=['app', 'lua'])
app = env.Program('app', app_objs, LIBS=['c', 'crt0', 'lua', 'm'], LIBPATH=['lib'], LINKFLAGS=LDFLAGS)

fw_objs = env.Object(fw_files, CCFLAGS=CFLAGS, CPPPATH=['fw', 'fat'])
fw = env.Program('firmware', fw_objs, LIBS=['m', 'fat'], LIBPATH=['lib'], LINKFLAGS=LDFLAGS)
fw_hex = env.Hex(fw)

#Depends(fw_hex, crt)
Default(fw_hex)