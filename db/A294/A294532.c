/*
 * A294532  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) Initial values: a(0)=1, a(1)=2,
 *            b(0)=3 The sequence a() is computed together with its
 *            complementary sequence b() by the recurrence shown above. The
 *            routine keeps a cache so that repeated calls to a(n) (in any
 *            order) are fast
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *a_cache = NULL;   /* cache of a(0)..a(k) */
static seq_t *b_cache = NULL;   /* cache of b(0)..b(k)  */
static size_t cache_len = 0;    /* number of a terms stored (indices 0..cache_len-1) */

/* binary search: returns true if val is present in a[0..upto] (inclusive) */
static bool contains_a(const seq_t *arr, size_t upto, seq_t val)
{
    size_t lo = 0, hi = upto;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return arr[lo] == val;
}

/* grow cache to include terms up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len) return;     /* already computed */

    size_t new_len = (size_t)n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_len * sizeof(seq_t));

    size_t old_len = cache_len;          /* last index that was valid before */
    for (idx_t i = (idx_t)old_len; i <= n; ++i) {
        size_t ui = (size_t)i;
        if (i == 0) {                   /* base values */
            a_cache[0] = (seq_t)1;
            b_cache[0] = (seq_t)3;
        } else if (i == 1) {
            a_cache[1] = (seq_t)2;
        } else {                        /* recurrence */
            seq_t ai = a_cache[ui - 1] + a_cache[ui - 2] + b_cache[ui - 2];
            a_cache[ui] = ai;

            /* find next complementary number b(i-1) */
            seq_t cand = b_cache[ui - 2] + (seq_t)1;
            while (contains_a(a_cache, ui, cand))
                cand++;
            b_cache[ui - 1] = cand;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[(size_t)n];
}
