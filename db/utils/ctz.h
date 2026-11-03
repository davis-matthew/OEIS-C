/* ctz: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_H
#define OEISC_UTILS_CTZ_H
#include <stddef.h>
#include <stdint.h>

/* --- helper: 2‑adic valuation for idx_t --------------------------------- */
static idx_t ctz(idx_t x)
{
    idx_t v = 0;
    while (x != 0 && (x & 1) == 0)
    {
        x >>= 1;
        ++v;
    }
    return v;
}

#endif
