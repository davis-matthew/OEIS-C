/*
 * A275134  Number of 5Xn 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +16*a(n-2) +49*a(n-3) +109*a(n-4) +31*a(n-5)
 *            -508*a(n-6) * +75*a(n-7) +324*a(n-8) -972*a(n-9) +260*a(n-10)
 *            for n > 14 * Initial values: a(1)=8, a(2)=108, a(3)=373,
 *            a(4)=2178, a(5)=12423, * a(6)=71576, a(7)=412306, a(8)=2381629,
 *            a(9)=13727957, * a(10)=79158451, a(11)=456572944, a(12)...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[14] = {
    (seq_t)8ULL,
    (seq_t)108ULL,
    (seq_t)373ULL,
    (seq_t)2178ULL,
    (seq_t)12423ULL,
    (seq_t)71576ULL,
    (seq_t)412306ULL,
    (seq_t)2381629ULL,
    (seq_t)13727957ULL,
    (seq_t)79158451ULL,
    (seq_t)456572944ULL,
    (seq_t)2633093481ULL,
    (seq_t)15185124732ULL,
    (seq_t)87573269532ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    if (cache == NULL) {
        /* initial capacity at least 16 to hold the seed and growth */
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* Ensure we have capacity for n */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (n > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill initial seed values once */
    if (cache_len == 0) {
        for (idx_t i = 1; i <= 14; ++i) cache[i] = init[i-1];
        cache_len = 14;
    }

    /* Compute up to requested n if needed */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 14) {
                cache[i] = init[i-1];
                continue;
            }
            /* recurrence for i > 14 */
            seq_t term = cache[i-1];                 /* a(n-1) */
            term += 16 * cache[i-2];                /* a(n-2) */
            term += 49 * cache[i-3];                /* a(n-3) */
            term += 109 * cache[i-4];               /* a(n-4) */
            term += 31 * cache[i-5];                /* a(n-5) */
            term -= 508 * cache[i-6];               /* a(n-6) */
            term += 75 * cache[i-7];                /* a(n-7) */
            term += 324 * cache[i-8];               /* a(n-8) */
            term -= 972 * cache[i-9];               /* a(n-9) */
            term += 260 * cache[i-10];              /* a(n-10) */
            cache[i] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
