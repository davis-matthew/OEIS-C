/* ilog2_u64: floor(log2(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ILOG2_U64_H
#define OEISC_UTILS_ILOG2_U64_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline int ilog2_u64(uint64_t p)         /* highest set bit index, -1 if p==0 */
{
    if (p == 0) return -1;
    return 63 - __builtin_clzll(p);
}

#endif
