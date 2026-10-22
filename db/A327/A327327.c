/*
 * A327327  Partial sums of the sum of nonpowers of 2 dividing n.
 * https://oeis.org/A327327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} ( sum of non‑powers of 2 dividing k )
 * Range:     u8 n <= 20, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function recomputes the needed values on demand using a simple
   sieve‑like accumulation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/pow2.h"
#else
#include "../utils/pow2.h"
#endif

/* Static storage for memoisation */
static idx_t   max_n    = 0;
static seq_t *sigma    = NULL;   /* sigma[k] = sum of all divisors of k */
static seq_t *prefix   = NULL;   /* a(k) values */

/* Free existing tables, if any */
static void free_tables(void)
{
#ifdef __cplusplus
    if (sigma)   free_seq_array(sigma);
    if (prefix)  free_seq_array(prefix);
#else
    if (sigma)   free(sigma);
    if (prefix)  free(prefix);
#endif
    sigma  = NULL;
    prefix = NULL;
    max_n  = 0;
}

/* Allocate tables big enough for indices up to n (inclusive) */
static void alloc_tables(idx_t n)
{
    free_tables();
    max_n = n;
#ifdef __cplusplus
    sigma   = alloc_seq_array((size_t)(n + 1));
    prefix  = alloc_seq_array((size_t)(n + 1));
#else
    sigma   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    prefix  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    /* initialise to zero – alloc_seq_array may already do this */
    for (idx_t i = 0; i <= n; ++i) {
        sigma[i]  = (seq_t)0;
        prefix[i] = (seq_t)0;
    }
}

/* Recompute tables up to n (if needed) */
static void recompute(idx_t n)
{
    if (n <= max_n) return;
    alloc_tables(n);

    /* Sieve computation of sigma (sum of divisors) */
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = d; m <= n; m += d) {
            sigma[m] += (seq_t)d;
        }
    }

    /* Build prefix sums: a(k) = a(k-1) + g(k) */
    prefix[0] = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Count exponent of 2 dividing k */
        idx_t  t = k;
        unsigned v = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++v;
        }
        seq_t power     = pow2(v + 1);          /* 2^(v+1) as seq_t */
        seq_t powers_sum = power - (seq_t)1;    /* sum of 1,2,...,2^v */
        seq_t g          = sigma[k] - powers_sum;
        prefix[k] = prefix[k - 1] + g;
    }
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n<1, but safe */
    recompute(n);
    return prefix[n];
}
