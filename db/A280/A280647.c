/*
 * A280647  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 7.
 * https://oeis.org/A280647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 854, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9957 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequentially search for integers k whose cube
   has an odd number of decimal digits and a middle digit 7.
   Caches found results to answer arbitrary queries a(n). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* return true if k^3 has an odd number of digits and the middle digit is 7 */
static bool cube_middle_is7(idx_t k)
{
    seq_t cube = (seq_t)k * k * k;      /* k³ */

    /* extract decimal digits from least significant to most */
    unsigned char digs[128];
    unsigned len = 0;
    seq_t t = cube;
    while (t != 0) {
        digs[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    if (len == 0) return false;   /* only k = 0 would reach this */

    if ((len & 1u) == 0) return false;   /* even number of digits */

    /* middle digit from the left equals digit at index len/2 from the right */
    return digs[len >> 1] == 7u;
}

/* static state for caching */
static seq_t *cache = NULL;     /* array of found k's */
static size_t cache_sz = 0;     /* current number of cached values */
static idx_t next_k = 1;        /* next k to test after last found */

/* external linkage function a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but return 0 */

    /* if we already have the value cached, return it */
    if (n <= cache_sz)
        return cache[n - 1];

    /* find more terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        if (cube_middle_is7(next_k)) {
            /* grow cache by one element */
            seq_t *new_cache = mem_resize(seq_t, cache, (cache_sz + 1));
            cache = new_cache;                /* may be NULL; let caller handle */
            cache[cache_sz++] = (seq_t)next_k; /* store found k */
        }
        ++next_k;
    }

    return cache[n - 1];
}
