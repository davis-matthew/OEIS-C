/*
 * A247728  Number of length 2+3 0..n arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 4*a(n-3) + 10*a(n-4) - 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - a(n-8) Initial values: a(1)=8, a(2)=90,
 *            a(3)=456, a(4)=1592, a(5)=4344, a(6)=10098, a(7)=20816,
 *            a(8)=39264
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache == NULL)
    {
        /* initialise cache with the first 8 values */
        cache_len = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)90;
        cache[2] = (seq_t)456;
        cache[3] = (seq_t)1592;
        cache[4] = (seq_t)4344;
        cache[5] = (seq_t)10098;
        cache[6] = (seq_t)20816;
        cache[7] = (seq_t)39264;
    }

    if (n == 0)
        return 0; /* undefined for n < 1, but return 0 for safety */

    /* expand cache if needed */
    if ((size_t)n > cache_len)
    {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL)
        {
            /* allocation failed; return 0 as a fallback (unlikely) */
            return 0;
        }
        cache = tmp;

        /* compute new terms up to n */
        for (size_t pos = cache_len; pos < new_len; ++pos)
        {
            /* pos is 0‑based index for a(pos+1) */
            seq_t plus  = 4 * cache[pos - 1] + 10 * cache[pos - 4] + 4 * cache[pos - 7];
            seq_t minus = 4 * cache[pos - 2] + 4 * cache[pos - 3]
                        + 4 * cache[pos - 5] + 4 * cache[pos - 6]
                        +       cache[pos - 8];
            cache[pos] = plus - minus;     /* result is guaranteed non‑negative */
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
