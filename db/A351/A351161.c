/*
 * A351161  G.f. A(x) satisfies: A(x) = x + x^2 * A(x/(1 - 6*x)) / (1 - 6*x).
 * https://oeis.org/A351161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(n) = Σ_{k=0..n-2} C(n-2,k) * 6^k *
 *            a(n-k-2) for n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes and static storage for memoisation */
#include <stdlib.h>
#include <stdint.h>

/* static cache for computed a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
/* cache for powers of 6: pow6[i] = 6^i */
static seq_t *pow6_cache = NULL;
static size_t pow6_cap = 0;
/* highest index for which a_cache has been filled */
static idx_t a_max_computed = 0;

/* Ensure that a_cache can hold values up to idx (inclusive) */
static void ensure_a_cache(idx_t idx)
{
    if (a_cache_cap <= (size_t)idx)
    {
        size_t newcap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        a_cache_cap = newcap;
    }
}

/* Ensure that pow6_cache can hold powers up to idx (inclusive) */
static void ensure_pow6_cache(idx_t idx)
{
    if (pow6_cap <= (size_t)idx)
    {
        size_t newcap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(pow6_cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        pow6_cache = tmp;
        pow6_cap = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 0;
    if (n == 1) {
        /* Ensure a_cache[1] exists for later calls */
        ensure_a_cache(1);
        a_cache[0] = 0;
        a_cache[1] = 1;
        a_max_computed = 1;
        return 1;
    }

    /* Allocate caches */
    ensure_a_cache(n);
    ensure_pow6_cache(n);

    /* Initialise powers of 6 up to n */
    pow6_cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow6_cache[i] = pow6_cache[i - 1] * (seq_t)6;

    /* Initialise base values if not already done */
    if (a_max_computed == 0)
    {
        a_cache[0] = 0;
        a_cache[1] = 1;
        a_max_computed = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t idx = a_max_computed + 1; idx <= n; ++idx)
    {
        idx_t m = idx - 2;          /* m = n-2 */
        seq_t sum = 0;
        seq_t binom = 1;            /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k)
        {
            seq_t term = binom * pow6_cache[k];
            term *= a_cache[m - k];
            sum += term;
            if (k < m)
            {
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1); /* exact division */
            }
        }
        a_cache[idx] = sum;
    }
    a_max_computed = n;
    return a_cache[n];
}
