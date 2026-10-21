/*
 * A289078  Number of orderless same-trees of weight n.
 * https://oeis.org/A289078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{d|n, d>1} binomial( a(n/d) + d - 1 , d )
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 71, u64 n <= 143,
 *            u128 n <= 287, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* forward declarations */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* ensure that cache has room for indices 0..n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* out of memory, terminate */
        exit(1);
    }

    /* initialise new slots to zero */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_size = new_size;
}

/* multiplicative binomial: C(N, K) where N is seq_t, K is idx_t */
static inline seq_t binom_seq(seq_t N, idx_t K)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t term = N - (seq_t)K + (seq_t)i;
        res = (res * term) / (seq_t)i;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* unused, but defined for safety */
        return 0;

    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];

    /* base case */
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }

    seq_t sum = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* divisor d */
        {
            idx_t m = n / d;
            seq_t a_m = a(m);
            seq_t N = a_m + (seq_t)d - 1;
            sum += binom_seq(N, d);
        }

        /* complementary divisor n/d, avoid double counting */
        idx_t d2 = n / d;
        if (d2 != d && d2 > 1) {
            idx_t m2 = n / d2;   /* equals d */
            seq_t a_m2 = a(m2);
            seq_t N2 = a_m2 + (seq_t)d2 - 1;
            sum += binom_seq(N2, d2);
        }
    }

    /* divisor d = n (always > 1 for n ≥ 2) */
    {
        idx_t d = n;
        seq_t a_1 = a(1);            /* a(1) = 1 */
        seq_t N = a_1 + (seq_t)d - 1;
        sum += binom_seq(N, d);
    }

    cache[n] = sum + 1;
    return cache[n];
}
