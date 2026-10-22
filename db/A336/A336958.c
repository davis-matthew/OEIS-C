/*
 * A336958  E.g.f.: 1 / (exp(2*x) - x).
 * https://oeis.org/A336958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336958: a(0)=1; a(n)= -n*a(n-1) - Σ_{k=2..n} binomial(n,k)*2^k*a(n-k) */

/* This implementation memoizes terms and computes them in O(n^2) time,
   suitable for n up to about 10 000.  It relies on the sequence allocation
   utilities provided in the build environment. */

#include <stddef.h>
#include <stdint.h>

/* Include the allocation and deallocation helpers for seq_t arrays. */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *seq_cache      = NULL;   /* cached sequence values */
static seq_t *pow2_cache     = NULL;   /* cached powers of two for indices 0..len-1 */
static idx_t  seq_cache_len  = 0;      /* current cache length (number of stored terms) */

/* Ensure that the cache contains terms up to index n. */
static void
ensure_cache(idx_t n)
{
    if (n < seq_cache_len)
        return;   /* already computed */

    idx_t new_len = n + 1;   /* we need indices 0..n inclusive */

    /* Allocate new arrays with the desired size. */
    seq_t *new_cache  = seq_alloc_new_array(new_len);
    seq_t *new_pow2   = seq_alloc_new_array(new_len);

    /* Copy any existing data into the new arrays. */
    for (idx_t i = 0; i < seq_cache_len; ++i) {
        new_cache[i] = seq_cache[i];
        new_pow2[i]  = pow2_cache[i];
    }

    /* Initialise missing powers of two. */
    if (seq_cache_len == 0) {
        new_pow2[0] = (seq_t)1;          /* 2^0 = 1 */
        for (idx_t i = 1; i < new_len; ++i)
            new_pow2[i] = new_pow2[i - 1] * (seq_t)2;
    } else {
        for (idx_t i = seq_cache_len; i < new_len; ++i)
            new_pow2[i] = new_pow2[i - 1] * (seq_t)2;
    }

    /* Compute new sequence terms. */
    for (idx_t m = seq_cache_len; m <= n; ++m) {
        if (m == 0) {                      /* a(0) = 1 */
            new_cache[0] = (seq_t)1;
        } else {
            seq_t sum = 0;                 /* Σ_{k=2..m} binomial(m,k)*2^k*a(m-k) */
            seq_t binom = (seq_t)m;        /* C(m,1) */

            for (idx_t k = 2; k <= m; ++k) {
                /* Update binomial coefficient C(m,k) from C(m,k-1). */
                binom = binom * (seq_t)(m - k + 1);
                binom = binom / (seq_t)k;          /* exact division */

                /* Term = C(m,k) * 2^k * a(m-k) */
                seq_t term = binom * new_pow2[k] * new_cache[m - k];
                sum += term;
            }

            /* Recurrence: a(m) = -m*a(m-1) - sum */
            new_cache[m] = -((seq_t)m * new_cache[m - 1] + sum);
        }
    }

    /* Free old arrays if they exist. */
    if (seq_cache != NULL)
        free_seq_array(seq_cache);
    if (pow2_cache != NULL)
        free_seq_array(pow2_cache);

    /* Store the newly computed arrays. */
    seq_cache      = new_cache;
    pow2_cache     = new_pow2;
    seq_cache_len  = new_len;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
