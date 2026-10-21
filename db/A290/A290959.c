/*
 * A290959  Matrix rank of the number of dots in the pairwise intersections of
 *          Ferrers diagrams.
 * https://oeis.org/A290959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1) + d(n) for n ≥ 5 with initial values a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=5. Here d(k) is the number of positive
 *            divisors of k (OEIS A000005)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;        /* array of computed a(i) values, 0‑based index */
static idx_t a_cache_size = 0;      /* current allocated size of a_cache */
static idx_t a_max_computed = 0;    /* largest n for which a_cache[n-1] is set */

/* divisor count array: div_counts[k] = number of divisors of k (k ≥ 1) */
static idx_t *div_counts = NULL;
static idx_t div_size = 0;           /* allocated size of div_counts array (≥ div_counts[0] unused) */

/* Ensure divisor counts are computed for all integers up to max_n (inclusive). */
static void compute_divisors(idx_t max_n)
{
    if (div_size <= max_n)
    {
        free(div_counts);
        div_counts = (idx_t *)calloc((size_t)(max_n + 1), sizeof(idx_t));
        div_size = max_n + 1;
    }
    /* reset counts */
    for (idx_t i = 1; i <= max_n; ++i)
        div_counts[i] = 0;
    /* sieve‑like counting of divisors */
    for (idx_t d = 1; d <= max_n; ++d)
    {
        for (idx_t j = d; j <= max_n; j += d)
            ++div_counts[j];
    }
}

/* Extend the cache to cover all values up to n (inclusive). */
static void extend_cache(idx_t n)
{
    if (n <= a_max_computed) return;

    /* Ensure divisor counts are ready */
    compute_divisors(n);

    /* Allocate or enlarge the a_cache array if necessary */
    idx_t new_size = n;                /* we store a(1)..a(n) */
    if (a_cache_size < new_size)
    {
        a_cache = (seq_t *)realloc(a_cache, (size_t)new_size * sizeof(seq_t));
        a_cache_size = new_size;
    }

    /* Compute missing entries from a_max_computed+1 up to n */
    for (idx_t i = a_max_computed + 1; i <= n; ++i)
    {
        if (i == 1)
            a_cache[i - 1] = (seq_t)1;
        else if (i == 2)
            a_cache[i - 1] = (seq_t)2;
        else if (i == 3)
            a_cache[i - 1] = (seq_t)3;
        else if (i == 4)
            a_cache[i - 1] = (seq_t)5;
        else
        {
            seq_t prev = a_cache[i - 2];      /* a(i-1) */
            seq_t add  = (seq_t)div_counts[i];/* d(i) */
            a_cache[i - 1] = prev + add;      /* a(i) = a(i-1) + d(i) */
        }
    }
    a_max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, but harmless */
    extend_cache(n);
    return a_cache[n - 1];
}
