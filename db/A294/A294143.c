/*
 * A294143  Sum of the larger parts of the partitions of 2n into two parts
 *          with smaller part squarefree.
 * https://oeis.org/A294143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (2*n - i) * mu(i)^2
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-degenerate: returns 0 for n == 0 */
#include <stdlib.h>
#include <stdint.h>

/* global cache */
static unsigned char *sqf = NULL;                /* 1 if squarefree, 0 otherwise */
static idx_t *pref_count = NULL;                /* count of squarefree numbers <= i */
static seq_t *pref_sum = NULL;                  /* sum of squarefree numbers <= i */
static idx_t max_computed = 0;

/* Extend precomputed tables up to n (inclusive). */
static void extend_idx(idx_t n) {
    if (n <= max_computed)
        return;
    size_t new_size = (size_t)n + 1;

    sqf = (unsigned char *)realloc(sqf, new_size * sizeof(unsigned char));
    pref_count = (idx_t *)realloc(pref_count, new_size * sizeof(idx_t));
    pref_sum = (seq_t *)realloc(pref_sum, new_size * sizeof(seq_t));
    if (!sqf || !pref_count || !pref_sum)
        exit(1);   /* memory allocation failure */

    if (max_computed == 0) {
        pref_count[0] = 0;
        pref_sum[0] = 0;
        sqf[0] = 0;
    }

    /* initialize new entries as squarefree */
    for (size_t i = max_computed + 1; i <= n; ++i)
        sqf[i] = 1;

    /* compute primes up to sqrt(n) */
    size_t sqrt_n = 1;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= n)
        ++sqrt_n;

    unsigned char *is_prime = (unsigned char *)malloc((sqrt_n + 1) * sizeof(unsigned char));
    if (!is_prime)
        exit(1);
    for (size_t i = 0; i <= sqrt_n; ++i)
        is_prime[i] = 1;
    for (size_t i = 2; i * i <= sqrt_n; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= sqrt_n; j += i)
                is_prime[j] = 0;

    /* mark non-squarefree numbers in the newly added range */
    for (size_t p = 2; p <= sqrt_n; ++p) {
        if (!is_prime[p])
            continue;
        size_t p2 = p * p;
        size_t start = ((max_computed + 1 + p2 - 1) / p2) * p2;
        for (size_t j = start; j <= n; j += p2)
            sqf[j] = 0;
    }
    free(is_prime);

    /* build prefix arrays for the new part */
    for (size_t i = max_computed + 1; i <= n; ++i) {
        pref_count[i] = pref_count[i - 1] + (sqf[i] ? 1 : 0);
        pref_sum[i]  = pref_sum[i - 1] + (sqf[i] ? (seq_t)i : 0);
    }
    max_computed = n;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for offset 1, but safe */
    extend_idx(n);
    idx_t S = pref_count[n];
    seq_t sum_i = pref_sum[n];
    seq_t two_n = (seq_t)n + (seq_t)n;
    seq_t result = two_n * S - sum_i;
    return result;
}
