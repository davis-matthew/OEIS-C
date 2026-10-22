/*
 * A347108  a(n) = Sum_{k=1..n} sigma(k)*sigma(2*k), where sigma(n) =
 *          A000203(n) is the sum of the divisors of n.
 * https://oeis.org/A347108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} sigma(k)*sigma(2*k)
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* where sigma(n) is the sum of all divisors of n */

/* Caches for sigma(k) and for the prefix sums a(k). */
static seq_t *sigma      = NULL;   /* sigma[0] unused, up to 2*last_n */
static size_t sigma_size = 0;      /* number of entries in sigma   */

static seq_t *a_cache    = NULL;   /* a_cache[0] unused, up to last_n */
static size_t a_cache_size = 0;    /* number of entries in a_cache */

static idx_t last_n = 0;           /* maximum n that has been computed */

/* Recompute the divisor‑sum array up to new_limit
   (new_limit must be non‑negative).  The old array is freed. */
static void recompute_sigma(idx_t new_limit)
{
    /* Allocate a zeroed array of (new_limit+1) entries. */
    seq_t *arr = (seq_t*)calloc((size_t)new_limit + 1, sizeof(seq_t));
    if (!arr) return;                 /* let the caller deal with OOM */
    for (idx_t i = 1; i <= new_limit; ++i) {
        for (idx_t j = i; j <= new_limit; j += i)
            arr[j] += (seq_t)i;
    }
    free(sigma);
    sigma = arr;
    sigma_size = (size_t)new_limit + 1;
}

/* Extend all caches so that they cover all indices up to n.
   After this call, a_cache[n] is guaranteed to be correct. */
static void extend_to(idx_t n)
{
    /* Ensure sigma is available for indices up to 2*n. */
    if ((size_t)(2 * n + 1) > sigma_size)
        recompute_sigma(2 * n);

    /* Grow the a_cache array if necessary. */
    if ((size_t)(n + 1) > a_cache_size) {
        a_cache = (seq_t*)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        a_cache_size = (size_t)(n + 1);
    }

    /* Compute a(k) for any missing k > last_n. */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        seq_t prod = sigma[k] * sigma[2 * k];
        if (k == 1) {
            a_cache[1] = prod;
        } else {
            a_cache[k] = a_cache[k - 1] + prod;
        }
    }
    last_n = n;
}

/* --------------------------------------------------------------------- */
/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined */
    extend_to(n);
    return a_cache[n];
}
