/*
 * A399355  Number of independent vertex sets in the 4 X n Moebius strip
 *          graph.
 * https://oeis.org/A399355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 20*a(n-2) + 27*a(n-3) - 14*a(n-4) - 25*a(n-5) +
 *            4*a(n-6) + 5*a(n-7) - a(n-8) /* Initial values: a(0)=2, a(1)=7,
 *            a(2)=27, a(3)=172, a(4)=863, a(5)=4907, a(6)=26262, a(7)=144655
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* grow_cache(seq_t *old, size_t old_len, size_t new_len)
{
#if defined(__cplusplus)
    seq_t* newp = new seq_t[new_len];
    if (old) {
        for (size_t i = 0; i < old_len; ++i) newp[i] = old[i];
        delete[] old;
    }
    return newp;
#else
    void* p = realloc(old, new_len * sizeof(seq_t));
    return (seq_t*)p;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;   /* number of terms stored */

    /* First call: initialize base terms */
    if (cache == NULL)
    {
        cache = grow_cache(NULL, 0, 8);
        if (cache == NULL) return 0;   /* allocation error, return 0 as sentinel */
        cache[0] = 2;   cache[1] = 7;   cache[2] = 27;
        cache[3] = 172; cache[4] = 863; cache[5] = 4907;
        cache[6] = 26262; cache[7] = 144655;
        cached = 8;
    }

    if (n < cached)
        return cache[n];

    size_t new_size = (size_t)n + 1;
    cache = grow_cache(cache, cached, new_size);
    if (cache == NULL) return 0;  /* allocation error */

    for (idx_t i = cached; i <= n; ++i)
    {
        seq_t val = cache[i-1];
        val += cache[i-2] * 20;
        val += cache[i-3] * 27;
        val -= cache[i-4] * 14;
        val -= cache[i-5] * 25;
        val += cache[i-6] * 4;
        val += cache[i-7] * 5;
        val -= cache[i-8];
        cache[i] = val;
    }
    cached = new_size;
    return cache[n];
}
