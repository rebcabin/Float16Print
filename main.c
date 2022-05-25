#include <stdio.h>
#include <stdint.h>

typedef union {
    struct {  // little-endian
        uint16_t m9: 9;
        uint16_t e6: 6;
        uint16_t s1: 1;
    } fields;
    uint16_t u16datum;
} GsiF16;

typedef union {
    struct {  // little-endian
        uint16_t mA: 10;  // A is hex for decimal 10.
        uint16_t e5:  5;
        uint16_t s1:  1;
    } fields;
    uint16_t u16datum;
} HalF16;  // IEEE 754r half-precision

typedef union {
    struct {  // little-endian
        uint32_t mW: 23;  // W is the 23-rd letter of the alphabet.
        uint32_t e8:  8;
        uint32_t s1:  1;
    } fields;
    uint32_t u32datum;
    float    f32datum;
} FloF32;

FloF32 floF32_from_halF16 (HalF16 halF16) {
    FloF32 result;
    result.fields.s1 = halF16.fields.s1;
    result.fields.e8 = halF16.fields.e5 << 3;
    result.fields.mW = halF16.fields.mA << 13;
    return result;
}

HalF16 halF16_from_floF32 (FloF32 floF32) {
    HalF16  result;
    result.fields.s1 = floF32.fields.s1;
    result.fields.e5 = floF32.fields.e8 >> 3;
    result.fields.mA = floF32.fields.mW >> 13;
    return result;
}

GsiF16 gsiF16_from_floF32 (FloF32 floF32) {
    GsiF16 result;
    result.fields.s1 = floF32.fields.s1;
    result.fields.e6 = floF32.fields.e8 >> 2;
    result.fields.m9 = floF32.fields.mW >> 14;
    return result;
}

FloF32 floF32_from_gsiF16 (GsiF16 gsiF16) {
    FloF32 result;
    result.fields.s1 = gsiF16.fields.s1;
    result.fields.e8 = gsiF16.fields.e6 << 2;
    result.fields.mW = gsiF16.fields.m9 << 14;
    return result;
}

int main() {
    FloF32 floF32;
    floF32.fields.s1 = 0;
    floF32.fields.e8 = 0x80;
    floF32.fields.mW = 0x490FBD;
    printf("fu32: 0x%08X, s1: 0x%08X, e8: 0x%08X, mW: 0x%08X, f: %f\n",
           floF32.u32datum,
           floF32.fields.s1, floF32.fields.e8, floF32.fields.mW,
           floF32.f32datum);

    HalF16 halF16 = halF16_from_floF32(floF32);
    printf("fu16: 0x%08X, s1: 0x%08X, e5: 0x%08X, mA: 0x%08X, f: %f\n",
           halF16.u16datum,
           halF16.fields.s1, halF16.fields.e5, halF16.fields.mA,
           floF32_from_halF16(halF16).f32datum);

    GsiF16 gsiF16 = gsiF16_from_floF32(floF32);
    printf("gu16: 0x%08X, s1: 0x%08X, e6: 0x%08X, m9: 0x%08X, f: %f\n",
           gsiF16.u16datum,
           gsiF16.fields.s1, gsiF16.fields.e6, gsiF16.fields.m9,
           floF32_from_gsiF16(gsiF16).f32datum);

    return 0;
}
