/*
 * A343687  a(0) = 1; a(n) = 4 * n * a(n-1) + Sum_{k=0..n-1} binomial(n,k) *
 *          (n-k-1)! * a(k).
 * https://oeis.org/A343687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 4*n*a(n-1) + Σ_{k=0}^{n-1} binomial(n,k) *
 *            (n-k-1)! * a(k) (n ≥ 1) This implementation uses memoisation and
 *            caches factorials up to the required index. The code works for
 *            all integer sizes supported by the build environment (uint8,
 *            uint16, uint32, uint64, uint128 or arbitrary‑pre...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The following helper is provided by the build environment. */
extern seq_t binomial(idx_t n, idx_t k);

/* Cache for the sequence values. */
static seq_t *a_cache = NULL;      /* a(n) values up to current maximum */
static size_t a_cap = 0;           /* allocated size of a_cache */
static idx_t  last_computed = 0;   /* largest n for which a_cache is valid */

/* Cache for factorials (0! … n!). */
static seq_t *fact_cache = NULL;   /* factorial values */
static size_t fact_cap = 0;        /* allocated size of fact_cache */
                                 /* fact_cap == m+1 means factorials 0…m are valid */

/* --- Helper: ensure a_cache holds values up to n (inclusive). --- */
static void ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */
        a_cap   = (size_t)(n + 1);
    } else if (a_cap <= (size_t)n) {
        a_cache = (seq_t *)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        a_cap   = (size_t)(n + 1);
    }
}

/* --- Helper: ensure fact_cache holds factorials 0! … n! --- */
static void ensure_fact_cache(idx_t n)
{
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        fact_cache[0] = (seq_t)1;          /* 0! = 1 */
        fact_cap   = 1;
    } else if (fact_cap <= (size_t)n) {
        fact_cache = (seq_t *)realloc(fact_cache, (size_t)(n + 1) * sizeof(seq_t));
    }

    /* Compute any missing factorials.  fact_cap == m+1 means factorials 0…m present. */
    for (size_t j = fact_cap; j <= (size_t)n; ++j) {
        fact_cache[j] = fact_cache[j - 1] * (seq_t)j;
    }
    fact_cap = (size_t)(n + 1);
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_a_cache(n);
    ensure_fact_cache(n);

    /* Compute any missing a(i) values up to n in ascending order. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* 4 * i * a(i-1) */
        seq_t term1 = (seq_t)4 * (seq_t)i * a_cache[i - 1];

        /* Sum over k = 0 .. i-1: C(i,k) * (i-k-1)! * a(k) */
        seq_t sum = (seq_t)0;
        seq_t C   = (seq_t)1;                     /* C(i,0) = 1 */

        for (idx_t k = 0; k < i; ++k) {
            seq_t fact = fact_cache[i - k - 1];
            sum += C * fact * a_cache[k];

            /* Update C to C(i,k+1) for next iteration */
            C = (C * (seq_t)(i - k)) / (seq_t)(k + 1);
        }

        a_cache[i] = term1 + sum;
    }

    last_computed = n;
    return a_cache[n];
}
