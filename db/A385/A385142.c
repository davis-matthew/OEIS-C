/*
 * A385142  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5) with a(1)
 *          = a(2) = a(3) = 0, a(4) = 1, and a(5) = 3.
 * https://oeis.org/A385142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5); Initial
 *            values: a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=3
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 75, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;            /* cache[i] = a(i+1) */
    static size_t cache_cap = 0;           /* allocated capacity (number of terms) */
    static idx_t cache_last = 0;           /* highest n cached so far */

    if (n <= cache_last)
        return cache[n - 1];              /* cached value */

    /* enlarge cache if needed */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp && (size_t)n > cache_cap) /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* compute terms from cache_last+1 up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i - 1] = (seq_t)0;
        } else if (i == 4) {
            cache[i - 1] = (seq_t)1;
        } else if (i == 5) {
            cache[i - 1] = (seq_t)3;
        } else {
            /* recurrence: 4*a(i-1) - 6*a(i-2) + 4*a(i-3) - a(i-4) + a(i-5) */
            seq_t t1 = cache[i - 2];   /* a(i-1) */
            seq_t t2 = cache[i - 3];   /* a(i-2) */
            seq_t t3 = cache[i - 4];   /* a(i-3) */
            seq_t t4 = cache[i - 5];   /* a(i-4) */
            seq_t t5 = cache[i - 6];   /* a(i-5) */

            seq_t res = (seq_t)4 * t1;
            res += (seq_t)4 * t3;
            res += t5;
            seq_t sub = (seq_t)6 * t2;
            sub += t4;
            res -= sub;

            cache[i - 1] = res;
        }
    }

    cache_last = n;
    return cache[n - 1];
}
