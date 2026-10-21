/*
 * A290270  Number of minimal dominating sets in the n-wheel graph.
 * https://oeis.org/A290270
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 66, u64 n <= 132,
 *            u128 n <= 263, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A290270: Number of minimal dominating sets in the n‑wheel graph
   Recurrence (valid for n > 8):
       a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6) - 1
   Initial values (offset 3):
       a(3)=3, a(4)=4, a(5)=7, a(6)=6, a(7)=6, a(8)=15
   The function is memoized and works for arbitrary‑precision seq_t. */
#include <stdlib.h>

static const seq_t INIT[9] = {0, 0, 0, 3, 4, 7, 6, 6, 15};

/* Compute a(n) for n >= 3.  For n < 3 the return value is undefined
   (the caller is guaranteed not to ask for such n). */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* value for offsets < 3 is irrelevant */

    /* Dynamic memoization cache: cache[i] holds a(i) for i = 0..max_computed. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current number of allocated slots (max i + 1) */

    if (cache_len <= n) {
        /* Grow the cache to hold entries up to n inclusive. */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            abort();          /* allocation failed */
        cache = tmp;

        /* Compute missing values from cache_len up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 8) {
                /* Use the stored initial values. */
                cache[i] = INIT[i];
            } else {
                /* Recurrence for i ≥ 9. */
                seq_t val = cache[i - 2];
                val += cache[i - 3];
                val += cache[i - 4];
                val -= cache[i - 6];
                val -= (seq_t)1;
                cache[i] = val;
            }
        }
        cache_len = (idx_t)new_len;
    }

    return cache[(size_t)n];
}
