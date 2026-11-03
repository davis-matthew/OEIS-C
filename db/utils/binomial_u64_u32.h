/* binomial_u64_u32: the binomial coefficient C(n, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BINOMIAL_U64_U32_H
#define OEISC_UTILS_BINOMIAL_U64_U32_H
#include <stddef.h>
#include <stdint.h>

/* capped combination: C(n,k) limited to 2^64-1 */
static uint64_t binomial_u64_u32(uint64_t n, uint32_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (uint32_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
        if (res > (uint64_t)-1) { res = (uint64_t)-1; break; }
    }
    return (uint64_t)res;
}

#endif
