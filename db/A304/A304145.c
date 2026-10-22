/*
 * A304145  Number of nX4 0..1 arrays with every element unequal to 1, 2, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-3) - 26*a(n-4) + 29*a(n-6) + 55*a(n-7) -
 *            18*a(n-8) - 111*a(n-9) - 15*a(n-10) + 60*a(n-11) + 72*a(n-12) -
 *            14*a(n-13) - 36*a(n-14) - 13*a(n-15) + 5*a(n-16) + 6*a(n-17) for
 *            n > 22. Initial values (n = 1 … 22): 2, 33, 25, 61, 157, 117,
 *            244, 936, 733, 1513, 5788, 5148, 9643, 36...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base[22] = {
    2, 33, 25, 61, 157, 117, 244, 936, 733, 1513,
    5788, 5148, 9643, 36537, 36384, 63504, 232477,
    257097, 425332, 1489062, 1805221, 2880515
};

static seq_t *cache = NULL;
static size_t cache_size = 0;

static void ensure_cache(idx_t n) {
    if (n <= cache_size) return;
    size_t new_cap = (size_t)n;
    if (new_cap < 22) new_cap = 22;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort */
        abort();
    }
    cache = new_cache;
    if (cache_size < 22) {
        size_t i;
        size_t end = new_cap < 22 ? new_cap : 22;
        for (i = cache_size; i < end; ++i) cache[i] = base[i];
    }
    size_t i;
    size_t startRec = cache_size > 22 ? cache_size : 22;
    for (i = startRec; i < new_cap; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        pos += cache[i - 1] * (seq_t)3;   /* a(n-1) */
        pos += cache[i - 3] * (seq_t)4;   /* a(n-3) */
        pos += cache[i - 6] * (seq_t)29;  /* a(n-6) */
        pos += cache[i - 7] * (seq_t)55;  /* a(n-7) */
        pos += cache[i -11] * (seq_t)60;  /* a(n-11) */
        pos += cache[i -12] * (seq_t)72;  /* a(n-12) */
        pos += cache[i -16] * (seq_t)5;   /* a(n-16) */
        pos += cache[i -17] * (seq_t)6;   /* a(n-17) */

        neg += cache[i - 4] * (seq_t)26;  /* a(n-4)  */
        neg += cache[i - 8] * (seq_t)18;  /* a(n-8)  */
        neg += cache[i - 9] * (seq_t)111; /* a(n-9)  */
        neg += cache[i -10] * (seq_t)15;  /* a(n-10) */
        neg += cache[i -13] * (seq_t)14;  /* a(n-13) */
        neg += cache[i -14] * (seq_t)36;  /* a(n-14) */
        neg += cache[i -15] * (seq_t)13;  /* a(n-15) */

        cache[i] = pos - neg;
    }
    cache_size = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset starts at 1 */
    if (n <= 22) return base[(size_t)n - 1];
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
