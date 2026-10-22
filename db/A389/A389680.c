/*
 * A389680  Numbers k for which no i > 1 exists such that k + i is composite
 *          and its least prime factor exceeds i^2.
 * https://oeis.org/A389680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS definition of A389680:
   Numbers k for which no i > 1 exists such that k + i is
   composite and its least prime factor exceeds i². */

/* Recommended for compiled as C11 or C++17. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/lpf.h"   /* idx_t lpf(idx_t n); */

#define CACHE_CAP 200000   /* maximum cached terms */

/* Static cache of computed terms.  Items after CACHE_CAP are
   not stored but can still be generated on demand. */
static seq_t  cache[CACHE_CAP];
static idx_t  cache_len = 0;   /* number of cached terms (≤ CACHE_CAP) */
static seq_t  last_term = 0;   /* last computed term, used when cache full */

/* Test whether a given k satisfies the property. */
static bool
ok_for_k(seq_t k)
{
    for (idx_t i = 2;; ++i) {
        seq_t m = k + (seq_t)i;          /* k + i */
        uint64_t m64 = (uint64_t)m;       /* assume fits in 64‑bit */

        /* If m overflows 64‑bit, we cannot apply lpf; stop testing. */
        if ((uint64_t)m != m64)
            break;

        idx_t sp = lpf(m64);              /* least prime factor of m */

        if (sp != m64 && (uint64_t)sp > (uint64_t)i * (uint64_t)i)
            return false;                 /* failure condition met */

        /* Stop when i⁴ > m ;  i⁴ fits in __int128 for i < 65536. */
        __int128 i4 = (__int128)i * i * i * i;
        if (i4 > m64)
            break;
    }
    return true;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but safe */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t k;
        if (cache_len == 0)
            k = 1;
        else
            k = last_term + 1;   /* start from the next integer */

        while (!ok_for_k(k))
            ++k;

        if (cache_len < CACHE_CAP)
            cache[cache_len++] = k;   /* cache if room left */

        last_term = k;
    }

    return (n <= CACHE_CAP) ? cache[n - 1] : last_term;
}
