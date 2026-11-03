/* odd_part_u64: n with its factors of 2 removed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ODD_PART_U64_H
#define OEISC_UTILS_ODD_PART_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t odd_part_u64(uint64_t x)
{
    while (x != 0 && (x & 1) == 0) x >>= 1;
    return x;
}

#endif
