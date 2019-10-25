import os

LD_FILE = 'fw/layout.ld'
CFLAGS = '-mcpu=cortex-m4 -mthumb -Wall -mfpu=fpv5-sp-d16 -mfloat-abi=hard -MMD'
LDFLAGS = '--specs=nano.specs -mcpu=cortex-m4 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -T ' + LD_FILE

hex_bld = Builder(action = 'arm-none-eabi-objcopy -O ihex $SOURCE $TARGET',
                suffix = '.hex',
                src_suffix = '.elf')

env = Environment(  ENV = os.environ,
                    CC = 'arm-none-eabi-gcc',
                    LINK = 'arm-none-eabi-gcc',
                    AR = 'arm-none-eabi-ar',
                    tools = ['mingw'],
                    PROGSUFFIX = '.elf',
                    BUILDERS = { 'Hex' : hex_bld })

fw_files = Glob('fw/*.c')
fat_files = Glob('fat/*.c')

fat_objs = env.Object(fat_files, CCFLAGS=CFLAGS, CPPPATH='fat')
fat = env.Library('fat', fat_objs, LINKFLAGS=LDFLAGS)

fw_objs = env.Object(fw_files, CCFLAGS=CFLAGS, CPPPATH='fw')
fw = env.Program('firmware', fw_objs, LIBS=['m', 'fat'], LIBPATH='.', LINKFLAGS=LDFLAGS)
fw_hex = env.Hex(fw)

Default(fw_hex)