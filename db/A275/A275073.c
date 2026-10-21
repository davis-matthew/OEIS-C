/*
 * A275073  Number of set partitions of [n] such that i-j is a multiple of six
 *          for all i,j belonging to the same block.
 * https://oeis.org/A275073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..5} A000110(floor((n+i)/6))
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 28, u64 n <= 44, u128 n <= 70,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000110 is the Bell number B(k) */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, exit, EXIT_FAILURE */
#include <stdbool.h>  /* bool */

/* Helper: compute binomial coefficient C(a,b) as seq_t */
#include "../utils/binom.h"

/* Static cache for Bell numbers */
static seq_t *bell_cache = NULL;   /* array of Bell numbers */
static size_t bell_cap   = 0;       /* allocated slots */
static size_t bell_len   = 0;       /* number of computed Bell values */

/* Ensure at least ‘need’ slots are allocated for bell_cache.  */
static void ensure_bell_capacity(size_t need)
{
    if (need <= bell_cap)
        return;
    size_t new_cap = bell_cap ? bell_cap * 2U : 1U;
    while (new_cap < need)
        new_cap <<= 1U;    /* double until large enough */
    seq_t *tmp = (seq_t *)realloc(bell_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort to avoid undefined behaviour. */
        exit(EXIT_FAILURE);
    }
    bell_cache = tmp;
    bell_cap   = new_cap;
}

/* Compute Bell numbers up to index ‘max_k’ (inclusive).  */
static void compute_bell_up_to(idx_t max_k)
{
    if (max_k < (idx_t)bell_len)
        return;   /* already computed up to this index */

    /* The values we compute are modest (~2000) so size_t suffices. */
    size_t max_idx = (size_t)max_k;

    ensure_bell_capacity(max_idx + 1U);

    if (bell_len == 0) {
        bell_cache[0] = (seq_t)1;
        bell_len = 1;
    }

    for (size_t n = bell_len; n <= max_idx; ++n) {
        seq_t sum = (seq_t)0;
        /* B(n) = Σ_{k=0}^{n-1} C(n-1, k) * B(k) */
        for (size_t k = 0; k < n; ++k) {
            seq_t term = binom((idx_t)(n - 1), (idx_t)k) * bell_cache[k];
            sum = sum + term;
        }
        bell_cache[n] = sum;
    }
    bell_len = max_idx + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The largest Bell index needed is ⌊(n+5)/6⌋.  */
    idx_t max_needed = (n + (idx_t)5) / (idx_t)6;
    compute_bell_up_to(max_needed);

    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 6; ++i) {
        idx_t idx = (n + i) / (idx_t)6;   /* ⌊(n+i)/6⌋ */
        result = result * bell_cache[(size_t)idx];
    }
    return result;
}
