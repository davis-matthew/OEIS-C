/*
 * A355805  Number of compositions (ordered partitions) of n into Pell numbers
 *          (A000129).
 * https://oeis.org/A355805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1
   a(n) = sum_{p <= n, p is a Pell number} a(n - p)   (n >= 1)   */
/* This DP uses a memoised cache; it works for any integer type `seq_t`
   (including arbitrary‑precision) and for any unsigned integer type `idx_t`. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cache for computed values of the sequence. */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   /* number of filled entries   */
static size_t cache_cap   = 0;   /* allocated capacity         */

/* Small pre‑allocated array for Pell numbers (as indices). */
static idx_t  pell[256];
static size_t pell_len = 0;

/* Generate Pell numbers up to `limit` (inclusive) and store them
   in the `pell` array (type idx_t).  The Pell sequence starts
   1, 2, 5, 12, 29, ..., recurrence p_k = 2*p_{k-1} + p_{k-2}. */
static void generate_pell_upto(idx_t limit)
{
    if (pell_len == 0) {          /* initialise first time            */
        pell[0] = (idx_t)1;
        pell[1] = (idx_t)2;
        pell_len = 2;
    }

    while (pell_len < 256 &&
           pell[pell_len - 1] <= limit)
    {
        idx_t next = pell[pell_len - 1] * (idx_t)2 + pell[pell_len - 2];
        pell[pell_len++] = next;
    }
}

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (cache_len <= n) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {           /* allocation failure   */
            return (seq_t)0;
        }
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base case a(0) if not yet set */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If already computed, return immediately */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Ensure Pell numbers up to n are available */
    generate_pell_upto(n);

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < pell_len && pell[j] <= (idx_t)i; ++j) {
            size_t idx = (size_t)(i - pell[j]);
            sum = sum + cache[idx];
        }
        cache[(size_t)i] = sum;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
