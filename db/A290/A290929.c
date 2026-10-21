/*
 * A290929  p-INVERT of the positive integers, where p(S) = (1 - S)(1 - S^2).
 * https://oeis.org/A290929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A290929
   Recurrence: a(0)=1, a(1)=4, a(2)=13, a(3)=39, a(4)=114, a(5)=330
               a(n)=7*a(n-1)-18*a(n-2)+23*a(n-3)-18*a(n-4)+7*a(n-5)-a(n-6)   (n>=6)
   This implementation memoises computed values in a static cache.
   It works for any seq_t that supports +,-,*,/ and uses malloc/realloc
   for dynamic array allocation.  */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        /* Compute missing terms */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 4;
            } else if (i == 2) {
                cache[i] = 13;
            } else if (i == 3) {
                cache[i] = 39;
            } else if (i == 4) {
                cache[i] = 114;
            } else if (i == 5) {
                cache[i] = 330;
            } else {
                cache[i] =
                    7 * cache[i-1]
                    - 18 * cache[i-2]
                    + 23 * cache[i-3]
                    - 18 * cache[i-4]
                    + 7 * cache[i-5]
                    - cache[i-6];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
