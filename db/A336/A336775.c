/*
 * A336775  a(n) is the largest power of n such that all numbers n^k <= a(n),
 *          k=1,..,A336774(n)-1 can be exactly represented as single precision
 *          32-bit floating point numbers according to the IEEE 754 standard.
 * https://oeis.org/A336775
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = n^(A336774(n)-1) where A336774(n) is the index of the
 *            largest power such that every n^k (k≤A336774(n)-1) is exactly
 *            representable as a IEEE‑754 binary32 floating point number
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 8929, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 2 */
    if (n < 2)
        return 0;

    /* Count the exponent of the factor 2 in n */
    idx_t v2 = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        ++v2;
        tmp >>= 1;
    }

    /* Current power value n^k, stored as an unsigned 128‑bit integer.
       The value never exceeds 2^128 − 1 for all relevant k. */
    unsigned __int128 cur = n;

    idx_t bestK = 0;          /* largest k for which all previous powers are valid */
    for (idx_t k = 1; ; ++k) {
        /* Compute floor(log2(cur)) – the exponent of the leading 1 in cur. */
        unsigned long long hi = (unsigned long long)(cur >> 64);
        unsigned long long lo = (unsigned long long)cur;
        int Lk;
        if (hi != 0) {
            Lk = 64 + 63 - __builtin_clzll(hi);
        } else {
            Lk = 63 - __builtin_clzll(lo);
        }

        /* 1. Exponent must not exceed the IEEE‑754 single‑precision limit. */
        if (Lk > 127)
            break;

        /* 2. At least Lk‑23 trailing zeros are required for exactness. */
        if (Lk > 23) {
            idx_t need = (idx_t)(Lk - 23);
            if (v2 * k < need)
                break;
        }

        bestK = k;   /* current k is valid */

        /* Prepare next value: n^(k+1) */
        /* Check for 128‑bit overflow before multiplying */
        unsigned __int128 max128 = (unsigned __int128)(-1);
        if (cur > max128 / n)   /* next multiplication would overflow */
            break;

        cur *= n;
    }

    /* Return the required power using the provided utility routine. */
    return pow_uint(n, bestK);
}
