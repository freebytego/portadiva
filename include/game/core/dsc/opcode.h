#ifndef DSC_OPCODE_H
#define DSC_OPCODE_H

#define DSC_OPCODE_TIME 0x01
#define DSC_OPCODE_TARGET 0x06
#define DSC_OPCODE_LYRIC 0x18
#define DSC_OPCODE_MUSIC_PLAY 0x19
#define DSC_OPCODE_BAR_TIME_SET 0x1C
#define DSC_OPCODE_TARGET_FLYING_TIME 0x3A

short get_parameters_count(int opcode)
{
  switch (opcode) {
    case 0x00: return 0;
    case DSC_OPCODE_TIME: return 1;
    case 0x02: return 4;
    case 0x03: return 2;
    case 0x04: return 2;
    case 0x05: return 2;
    case DSC_OPCODE_TARGET: return 7;
    case 0x07: return 4;
    case 0x08: return 2;
    case 0x09: return 6;
    case 0x0A: return 2;
    case 0x0B: return 1;
    case 0x0C: return 6;
    case 0x0D: return 2;
    case 0x0E: return 1;
    case 0x0F: return 1;
    case 0x10: return 3;
    case 0x11: return 2;
    case 0x12: return 3;
    case 0x13: return 5;
    case 0x14: return 5;
    case 0x15: return 4;
    case 0x16: return 4;
    case 0x17: return 5;
    case DSC_OPCODE_LYRIC: return 2;
    case DSC_OPCODE_MUSIC_PLAY: return 0;
    case 0x1A: return 2;
    case 0x1B: return 4;
    case DSC_OPCODE_BAR_TIME_SET: return 2;
    case 0x1D: return 2;
    case 0x1E: return 1;
    case 0x1F: return 21;
    case 0x20: return 0;
    case 0x21: return 3;
    case 0x22: return 2;
    case 0x23: return 5;
    case 0x24: return 1;
    case 0x25: return 1;
    case 0x26: return 7;
    case 0x27: return 1;
    case 0x28: return 1;
    case 0x29: return 2;
    case 0x2A: return 1;
    case 0x2B: return 2;
    case 0x2C: return 1;
    case 0x2D: return 2;
    case 0x2E: return 3;
    case 0x2F: return 3;
    case 0x30: return 1;
    case 0x31: return 2;
    case 0x32: return 2;
    case 0x33: return 3;
    case 0x34: return 6;
    case 0x35: return 6;
    case 0x36: return 1;
    case 0x37: return 1;
    case 0x38: return 2;
    case 0x39: return 3;
    case DSC_OPCODE_TARGET_FLYING_TIME: return 1;
    case 0x3B: return 2;
    case 0x3C: return 2;
    case 0x3D: return 4;
    case 0x3E: return 4;
    case 0x3F: return 1;
    case 0x40: return 2;
    case 0x41: return 1;
    case 0x42: return 2;
    case 0x43: return 1;
    case 0x44: return 1;
    case 0x45: return 3;
    case 0x46: return 3;
    case 0x47: return 3;
    case 0x48: return 2;
    case 0x49: return 1;
    case 0x4A: return 9;
    case 0x4B: return 3;
    case 0x4C: return 2;
    case 0x4D: return 4;
    case 0x4E: return 2;
    case 0x4F: return 3;
    case 0x50: return 2;
    case 0x51: return 24;
    case 0x52: return 1;
    case 0x53: return 2;
    case 0x54: return 1;
    case 0x55: return 3;
    case 0x56: return 1;
    case 0x57: return 3;
    case 0x58: return 4;
    case 0x59: return 1;
    case 0x5A: return 2;
    case 0x5B: return 6;
    case 0x5C: return 3;
    case 0x5D: return 2;
    case 0x5E: return 3;
    case 0x5F: return 3;
    case 0x60: return 4;
    case 0x61: return 1;
    case 0x62: return 1;
    case 0x63: return 3;
    case 0x64: return 3;
    case 0x65: return 4;
    case 0x66: return 2;
    case 0x67: return 3;
    case 0x68: return 3;
    case 0x69: return 8;
    case 0x6A: return 2;
    default: return -1;
  }
}

#endif
