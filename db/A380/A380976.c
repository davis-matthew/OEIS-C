/*
 * A380976  a(0) = 0, a(1) = 1. Thereafter, a(n) = a(n-1) + a(n-2) converted
 *          to base n, read in base n+1.
 * https://oeis.org/A380976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380976
   a(0) = 0, a(1) = 1
   For n ≥ 2: a(n) = value of a(n‑1)+a(n‑2) when its digits in base n are
   interpreted in base n+1.  */
#include <stdlib.h>

/* Cache to allow arbitrary order of calls. */
static seq_t *cache   = NULL;   /* array of computed a(n) values */
static size_t cache_cap = 0;    /* allocated array capacity */
static idx_t  cache_len  = 0;   /* last filled index + 1 */

/* Ensure the cache can hold entries up to index `n` (inclusive). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = needed;
    }
}

/* Return a(n) according to the recurrence. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache_size(n);

    /* Initialise cache if empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }
    if (cache_len == 1 && cache_cap > 1) {
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute all missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2];

        /* Convert `sum` from base `i` to base `i+1`. */
        seq_t result  = 0;
        seq_t mult    = 1;
        seq_t tmp     = sum;
        seq_t base    = (seq_t)i;        /* base i   */
        seq_t base_next = (seq_t)(i + 1);/* base i+1 */

        while (tmp > 0) {
            seq_t digit = tmp % base;
            result += digit * mult;
            tmp /= base;
            mult *= base_next;
        }

        cache[i] = result;
    }
    cache_len = n + 1;

    return cache[n];
}
