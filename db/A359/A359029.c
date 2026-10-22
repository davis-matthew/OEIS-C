/*
 * A359029  Integers m such that A006218(m+1)/(m+1) < A006218(m)/m.
 * https://oeis.org/A359029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359029: 
 *   m such that A006218(m+1)/(m+1) < A006218(m)/m,
 *   i.e. f(m+1) < f(m) where f(m)=∑_{k=1..m}d(k)/m.
 *   The function computes the sequence by enumerating m and
 *   checking the inequality using the running sum of divisor
 *   counts.  It memoises results so repeated calls are fast.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------ helper: divisor count for a single number ------ */
static seq_t divisor_count(idx_t n)
{
    seq_t count = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            ++count;                          /* i divides n   */
            if (i != n / i) ++count;           /* (n/i) distinct */
        }
    }
    return count;
}

/* ------ static state for memoisation ------ */
static seq_t *cache = NULL;          /* stored terms of the sequence   */
static size_t cache_len = 0;         /* number of terms cached         */
static size_t cache_cap = 0;         /* capacity of cache array        */

static seq_t sum_divs = 1;           /* running sum ∑_{k=1..m}d(k)    */
static idx_t curr_m = 1;             /* m whose sum is sum_divs       */

/* Ensure cache has capacity at least 'need' */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Generate enough terms to satisfy 'need' terms in the sequence */
static void generate_terms(size_t need)
{
    if (cache_len >= need)
        return;

    ensure_cache_cap(need);

    /* On first call, initialise state */
    if (cache_len == 0) {
        sum_divs = 1;   /* d(1) = 1 */
        curr_m   = 1;
    }

    while (cache_len < need) {
        /* Compute d(m+1) */
        seq_t d_next = divisor_count(curr_m + 1);

        /* Condition f(m+1) < f(m)  ⇔  sum_divs > curr_m * d_next */
        if (sum_divs > (seq_t)curr_m * d_next) {
            cache[cache_len++] = curr_m;
        }

        /* Advance to next m */
        sum_divs += d_next;
        ++curr_m;
    }
}

/* Public OEIS function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined offset, safe return */

    generate_terms((size_t)n);
    return cache[(size_t)(n - 1)];
}
