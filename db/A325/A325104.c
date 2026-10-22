/*
 * A325104  Number of increasing pairs of positive integers up to n with at
 *          least one binary carry.
 * https://oeis.org/A325104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of increasing pairs of positive integers (i,j), 1
 *            ≤ i < j ≤ n, such that i & j != 0 (a bitwise overlap in binary).
 *            Implementation: cache values and extend the cache incrementally.
 *            For each new value n we add the contributions of pairs (k,n)
 *            with 1 ≤ k < n and k & n != 0. The running...
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /*  cached a(n) values   */
static idx_t max_computed = 0;  /*  current maximum n cached */

seq_t a(idx_t n)
{
    /* Allocate or extend the cache up to n.  The array always keeps
       a[0] through a[max_computed].  The size of the cache is
       (max_computed + 1).  We never shrink the cache.          */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (cache == NULL) {            /*  handle allocation failure   */
            return 0;
        }
        cache[0] = 0;
        max_computed = 0;
    } else if (n > max_computed) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) {
            /*  keep the previous allocation; return whatever is available  */
            return 0;
        }
        cache = tmp;
        /*  For each new index i > max_computed compute the increment
            contributed by pairs (k,i) with k < i.                          */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t add = 0;
            for (idx_t k = 1; k < i; ++k) {
                if (i & k) {
                    add++;
                }
            }
            /*  a(i) = a(i-1) + add  .  The case i==0 is already handled.  */
            cache[i] = cache[i - 1] + add;
        }
        max_computed = n;
    }

    return cache[n];
}
