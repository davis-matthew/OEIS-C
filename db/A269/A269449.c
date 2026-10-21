/*
 * A269449  The first of 33 consecutive positive integers the sum of the
 *          squares of which is a square.
 * https://oeis.org/A269449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 46*a(n-6) - a(n-12) + 704 (n > 12) Initial values:
 *            a(1)=7, a(2)=27, a(3)=60, a(4)=181, a(5)=227, a(6)=612,
 *            a(7)=1085, a(8)=1985, a(9)=3492, a(10)=9047, a(11)=11161,
 *            a(12)=28860
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 65, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values (a[0] = a(1)) */
    static const seq_t base[12] = {
        (seq_t)7,    (seq_t)27,   (seq_t)60,
        (seq_t)181,  (seq_t)227,  (seq_t)612,
        (seq_t)1085, (seq_t)1985, (seq_t)3492,
        (seq_t)9047, (seq_t)11161,(seq_t)28860
    };

    /* if n <= 12 we can return directly */
    if (n <= 12)
        return base[(size_t)(n - 1)];

    /* memoisation array for computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    size_t idx = (size_t)n;          /* adjust for 0‑based array */

    if (cache == NULL) {             /* allocate for the base terms */
        cache_cap = idx > 12 ? idx : 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory, unlikely in test */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = base[i];
        cache_len = 12;
    }

    if (idx > cache_len) {           /* extend cache to required size */
        if (idx > cache_cap) {
            cache_cap = idx;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* out of memory */
            cache = tmp;
        }
        for (size_t i = cache_len; i < idx; ++i) {
            /* recurrence: a(n) = 46*a(n-6) - a(n-12) + 704 */
            cache[i] = (seq_t)46 * cache[i - 6]
                       - cache[i - 12]
                       + (seq_t)704;
        }
        cache_len = idx;
    }

    return cache[idx - 1];
}
