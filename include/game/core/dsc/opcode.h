#ifndef DSC_OPCODE_H
#define DSC_OPCODE_H

#define DSC_OPCODE_TIME 0x01
#define DSC_OPCODE_TARGET 0x06
#define DSC_OPCODE_LYRIC 0x18
#define DSC_OPCODE_MUSIC_PLAY 0x19
#define DSC_OPCODE_BAR_TIME_SET 0x1C
#define DSC_OPCODE_TARGET_FLYING_TIME 0x3A

short opcode_get_parameters_count(int opcode);

#endif
