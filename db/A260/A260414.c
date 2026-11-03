/*
 * A260414  Expansion of psi(x^3) * psi(x^6) / f(-x^4) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A260414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 156, u32 n <= 466, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// /* OEIS A260414: expansion of psi(x^3)*psi(x^6)/f(-x^4) */
#include <stdlib.h>
#include "../utils/mem.h"

/* compute the series coefficients up to (inclusive) N */
static void compute_series(idx_t N, seq_t *arr)
{
    arr[0] = 1;
    for (idx_t i = 1; i <= N; ++i)
        arr[i] = 0;

    for (idx_t k = 1; k <= N; ++k) {
        /* exponent of (1 - x^k) in the product:
         * +1 from eta(q^6)   when 6 | k
         * +2 from eta(q^12)^2 when 12 | k
         * -1 from eta(q^3)   when 3  | k
         * -1 from eta(q^4)   when 4  | k
         */
        int e = ((k % 6  == 0) ? 1 : 0)
              + 2 * ((k % 12 == 0) ? 1 : 0)
              - ((k % 3  == 0) ? 1 : 0)
              - ((k % 4  == 0) ? 1 : 0);

        if (e == 1) {                       /* multiply by (1 - x^k)          */
            for (idx_t i = N; i >= k; --i)
                arr[i] -= arr[i - k];
        } else if (e == -1) {               /* multiply by (1 - x^k)^-1       */
            for (idx_t i = k; i <= N; ++i)
                arr[i] += arr[i - k];
        }
    }
}

static seq_t *cache = NULL;    /* array holding computed coefficients */
static idx_t cache_max = 0;    /* largest index that has been computed */

seq_t a(idx_t n)
{
    if (!cache || n > cache_max) {
        /* allocate enough space for indices [0 .. n] */
        seq_t *new_cache = mem_new(seq_t, (n + 1));
        if (!new_cache) return (seq_t)0;   /* out of memory */

        compute_series(n, new_cache);

        mem_free(cache);
        cache = new_cache;
        cache_max = n;
    }
    return cache[n];
}
