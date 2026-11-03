/* popcount_u8: the number of 1 bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POPCOUNT_U8_H
#define OEISC_UTILS_POPCOUNT_U8_H
#include <stdbool.h>
#include <stdint.h>

/* Byte pop‑count using Brian‑Kernighan's algorithm */
static inline unsigned popcount_u8(uint8_t b)
{
    unsigned c = 0;
    while (b) {
        c += 1;
        b &= b - 1;   /* clear the lowest set bit */
    }
    return c;
}

#endif
