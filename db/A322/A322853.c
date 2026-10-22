/*
 * A322853  Number of compositions (ordered partitions) of n into pentagonal
 *          pyramidal numbers (A002411).
 * https://oeis.org/A322853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{p∈P, p≤n} a(n-p),
   where P = { k^2*(k+1)/2 : k ≥ 1 } (pentagonal pyramidal numbers). */

#include <stdlib.h>
#include <stdint.h>

/* Static cached tables */
static seq_t *dp = NULL;        /* dynamic programming array */
static size_t dp_size = 0;      /* number of computed dp entries (dp[0] … dp[dp_size-1]) */
static idx_t *pent = NULL;      /* pentagonal pyramidal numbers */
static size_t pent_count = 0;   /* number of stored pent numbers */

/* Ensure that pent[] contains all numbers ≤ limit. */
static void ensure_pents_up_to(idx_t limit)
{
    /* Count how many values of k^2*(k+1)/2 are ≤ limit. */
    idx_t k = 1;
    while (1) {
        __int128 v = (__int128)k * k * (k + 1) / 2;
        if (v > limit) break;
        ++k;
    }
    size_t needed = (size_t)(k - 1);   /* number of entries needed */

    if (pent_count < needed) {
        /* Reallocate pent array if necessary */
        idx_t *tmp = (idx_t *)realloc(pent, needed * sizeof(idx_t));
        if (!tmp) abort();            /* allocation failure */
        pent = tmp;
        /* Compute the new entries */
        for (size_t i = pent_count; i < needed; ++i) {
            idx_t kk = (idx_t)(i + 1);
            __int128 v = (__int128)kk * kk * (kk + 1) / 2;
            pent[i] = (idx_t)v;
        }
        pent_count = needed;
    }
}

/* Ensure that dp[] contains all values up to and including limit.
   dp[0] is set to 1 (empty composition). */
static void ensure_dp_up_to(idx_t limit)
{
    /* First allocation: dp[0] = 1 */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!dp) abort();
        dp[0] = 1;
        dp_size = 1;
    }

    /* Extend the array if needed */
    if (dp_size <= (size_t)limit) {
        size_t new_size = (size_t)limit + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        dp = tmp;
        /* Compute new dp values */
        for (idx_t i = (idx_t)dp_size; i <= limit; ++i) {
            seq_t sum = 0;
            for (size_t j = 0; j < pent_count && pent[j] <= i; ++j) {
                sum += dp[(size_t)(i - pent[j])];
            }
            dp[(size_t)i] = sum;
        }
        dp_size = new_size;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    /* Compute required tables up to n */
    ensure_pents_up_to(n);
    ensure_dp_up_to(n);
    return dp[(size_t)n];
}
