/*
 * A373908  Number of compositions of 7*n into parts 2 and 7.
 * https://oeis.org/A373908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=9, a(4)=38, a(5)=136, a(6)=452
   a(n) = 7*a(n-1) - 20*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
           - 7*a(n-6) + a(n-7)   for n ≥ 7
   This function memoises the values in a static array so that any order of
   calls is fast and the sequence can be computed for n up to ~10000
   within the time limit.                                                    */

#include <stdlib.h>

/* External types defined by the build system: seq_t, idx_t */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of values a[0 .. cache_len-1] */
    static idx_t cache_len = 0;   /* current allocated length */

    /* Grow the cache if needed */
    if (cache_len <= n) {
        size_t old_len = (size_t)cache_len;
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        cache = tmp;

        /* base values for 0 ≤ i ≤ 6 */
        static const seq_t base[7] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                       (seq_t)9, (seq_t)38, (seq_t)136,
                                       (seq_t)452 };

        /* compute new entries from old_len through n */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i <= 6) {
                cache[i] = base[i];
            } else {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                seq_t a5 = cache[i-5];
                seq_t a6 = cache[i-6];
                seq_t a7 = cache[i-7];

                /* positive part: 7*a1 + 35*a3 + 21*a5 + a7  */
                seq_t pos = (seq_t)7 * a1 + (seq_t)35 * a3
                            + (seq_t)21 * a5 + a7;
                /* negative part: 20*a2 + 35*a4 + 7*a6          */
                seq_t neg = (seq_t)20 * a2 + (seq_t)35 * a4
                            + (seq_t)7 * a6;
                cache[i] = pos - neg;   /* guaranteed non‑negative */
            }
        }
        cache_len = (idx_t)new_len;
    }
    return cache[n];
}
