/*
 * A307800  Binomial transform of least common multiple sequence (A003418),
 *          starting with a(1).
 * https://oeis.org/A307800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k) * lcm(1..k+1) (A003418)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                /* for malloc/realloc */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t, seq_t) */

static seq_t *lcm_cache = NULL;   /* lcm_cache[i] = lcm(1..i), i ≥ 1 */
static size_t lcm_cap   = 0;       /* allocated capacity of lcm_cache */
static idx_t  lcm_max   = 0;       /* highest i for which lcm_cache[i] is valid */

/* Computes a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* Ensure lcm_cache can hold indices up to n+1. */
    if ((idx_t)lcm_cap <= n + 1) {
        size_t new_cap = (size_t)(n + 2);          /* indices 0..n+1 */
        seq_t *tmp = (seq_t *)realloc(lcm_cache, new_cap * sizeof(seq_t));
        lcm_cache = tmp;
        lcm_cap   = new_cap;
    }

    /* Compute missing lcm values up to n+1. */
    if (lcm_max < n + 1) {
        for (idx_t m = lcm_max + 1; m <= n + 1; ++m) {
            if (m == 1) {
                lcm_cache[1] = (seq_t)1;           /* lcm(1..1) = 1 */
            } else {
                seq_t prev = lcm_cache[m - 1];
                seq_t g    = gcd(prev, (seq_t)m);
                lcm_cache[m] = (prev / g) * (seq_t)m;
            }
        }
        lcm_max = n + 1;
    }

    /* Sum binomial(n,k) * lcm(1..k+1) for k = 0..n. */
    seq_t sum = 0;
    seq_t bin = 1;   /* binomial(n,0) */

    /* k = 0 term (lcm(1..1)) */
    sum += lcm_cache[1];

    for (idx_t k = 1; k <= n; ++k) {
        bin = (bin * (seq_t)(n - k + 1)) / (seq_t)k; /* binomial(n,k) */
        sum += bin * lcm_cache[k + 1];
    }

    return sum;
}
