/*
 * A296124  Number of n X 4 0..1 arrays with each 1 adjacent to 2 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A296124
   Empirical binomial recurrence:
        a(1)=1, a(2)=13, a(3)=30, a(4)=74, a(5)=287,
        a(6)=841, a(7)=2463, a(8)=7953, a(9)=24428,
        a(n) = 3*a(n-1) + a(n-2) + 5*a(n-3)
             - 18*a(n-4) - 14*a(n-5)
             + 3*a(n-6) + a(n-7) + a(n-8) - a(n-9)   (n≥10)
   The implementation memoises previous values so calls may arrive in
   any order.  All arithmetic is performed in the native seq_t
   representation; overflow for small seq_t types is expected to be
   modulo 2ⁿ and is acceptable as the sequence values then overflow
   naturally.  For arbitrary‑precision seq_t the arithmetic is exact. */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL; /* 0‑indexed array, 0 unused */
static size_t cap   = 0;    /* allocated size */
static size_t used  = 0;    /* largest n computed so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                              /* offset starts at 1 */

    /* If we already have the value, return it */
    if (n <= used) return cache[(size_t)n];

    /* Grow the cache if needed (index 0 unused, so size = n+1) */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – unrealistic for test env */ }
        cache = tmp;
        cap = newcap;
    }

    /* Known initial values (indexed from 1) */
    static const seq_t init[10] = {
        0,          /* index 0 unused */
        1ULL, 13ULL, 30ULL, 74ULL, 287ULL,
        841ULL, 2463ULL, 7953ULL, 24428ULL
    };

    /* Compute all values up to n */
    for (size_t k = used + 1; k <= (size_t)n; ++k) {
        if (k <= 9) {
            cache[k] = init[k];
        } else {
            seq_t term = 3 * cache[k-1] + cache[k-2] + 5 * cache[k-3]
                       + 3 * cache[k-6] + cache[k-7] + cache[k-8];
            term -= 18 * cache[k-4] + 14 * cache[k-5] + cache[k-9];
            cache[k] = term;
        }
    }
    used = (size_t)n;

    return cache[(size_t)n];
}
