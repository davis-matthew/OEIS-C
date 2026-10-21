/*
 * A264203  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having index change +-(.,.) 0,0 0,2 or 1,0.
 * https://oeis.org/A264203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 44*a(n-2) - 44*a(n-3) - 11*a(n-4) + a(n-5)
 *            (empirical), with initial values: a(1) = 81, a(2) = 1024, a(3) =
 *            14641, a(4) = 202500, a(5) = 2825761
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types provided by the build */
extern void *seq_realloc(void *, size_t); /* placeholder, actual realloc used */

/* Memoized recurrence implementation for A264203 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;

    if (n == 0)
        return (seq_t)0;          /* value outside the offset, harmless */

    if (n > cached_max) {
        size_t newsize = (size_t)(n + 1);
        seq_t *newp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (newp == NULL) {
            /* handle allocation failure (unlikely in typical use) */
            abort();
        }
        cache = newp;

        /* initialize from previous max+1 up to n */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[i] = (seq_t)81; break;
                case 2: cache[i] = (seq_t)1024; break;
                case 3: cache[i] = (seq_t)14641; break;
                case 4: cache[i] = (seq_t)202500; break;
                case 5: cache[i] = (seq_t)2825761; break;
                default:
                    /* compute using recurrence, using only + and - to avoid
                       unsigned underflow on negative literals */
                    seq_t t1 = (seq_t)11 * cache[i - 1];
                    seq_t t2 = (seq_t)44 * cache[i - 2];
                    seq_t t3 = (seq_t)44 * cache[i - 3];
                    seq_t t4 = (seq_t)11 * cache[i - 4];
                    cache[i] = t1 + t2 - t3 - t4 + cache[i - 5];
                    break;
            }
        }
        cached_max = n;
    }
    return cache[n];
}
