import os

LD_FILE = 'layout.ld'
CFLAGS = '-mcpu=cortex-m4 -mthumb -Wall -mfpu=fpv5-sp-d16 -mfloat-abi=hard -MMD'
LDFLAGS = '--specs=nano.specs -mcpu=cortex-m4 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -T ' + LD_FILE
print(LDFLAGS)

env = Environment(  ENV = os.environ,
                    CC='arm-none-eabi-gcc',
                    LD='arm-none-eabi-gcc',
                    CCFLAGS='-mcpu=cortex-m4 -mthumb -Wall -mfpu=fpv5-sp-d16 -mfloat-abi=hard -MMD')

fw_files = Glob('fw/*.c')
fat_files = Glob('fat/*.c')

fat_objs = env.Object(fat_files, CCFLAGS=CFLAGS, CPPPATH='fat')
fat = env.Library('fat', fat_objs, CCFLAGS=LDFLAGS)

fw_objs = env.Object(fw_files, CCFLAGS=CFLAGS, CPPPATH='fw')
fw = env.Program('firmware', fw_objs, LIBS=['m'], LIBPATH='.', CCFLAGS=LDFLAGS)

Default(fw)