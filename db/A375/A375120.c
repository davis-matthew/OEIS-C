/*
 * A375120  Number of complete binary unordered tree-factorizations of n.
 * https://oeis.org/A375120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A375120: Number of complete binary unordered tree‑factorizations of n.
   Recurrence:
     a(1) = 1
     a(p) = 1  for prime p
     for composite n
       a(n) = Σ_{d|n, 2 ≤ d < sqrt(n)} a(d)·a(n/d)
              + [n is a perfect square]·C(a(√n)+1, 2)   */
/* Note:  a(d)·a(n/d) is computed in type seq_t; binomial is (a+1)*a/2.  */

#include <stdbool.h>
#include <stddef.h>

/* Utility helpers provided by the build system. */
#include "../utils/ensure_cache.h"   /* void ensure_cache(idx_t n, seq_t **, size_t *); */
#include "../utils/isPrimeTrial.h"   /* bool isPrimeTrial(seq_t); */

/* Integer square‑root for 64‑bit values.  No overflow occurs because
   the multiplication is performed in unsigned __int128. */
static idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    /* Increase r while (r+1)*(r+1) ≤ n.  */
    for (;;) {
        idx_t t = r + 1;
        unsigned __int128 sq = (unsigned __int128)t * t;
        if (sq > n) break;
        r = t;
    }
    return r;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    /* Static cache – index 0 is unused; values equal to 0 mean “not yet
       computed”.  The sequence never takes the value 0 for n ≥ 1. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Make sure the cache can hold the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)                     /* already computed */
        return cache[n];

    /* Base cases. */
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }
    if (isPrimeTrial((seq_t)n)) {          /* prime */
        cache[n] = 1;
        return 1;
    }

    /* Composite case.  Sum over unordered divisor pairs. */
    seq_t sum = 0;
    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t d = 2; d <= sqrt_n; ++d) {
        if (n % d != 0) continue;          /* not a divisor */

        idx_t other = n / d;
        if (d < other) {                   /* distinct pair */
            sum += a(d) * a(other);
        } else if (d == other) {           /* perfect‑square split */
            seq_t val = a(d);
            /* binomial(val+1, 2) = (val+1)*val/2 */
            sum += (val + 1) * val / 2;
        }
    }

    cache[n] = sum;
    return sum;
}
