/*
 * A313161  Coordination sequence Gal.5.237.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-2) + a(n-3) -2*a(n-4) + 3*a(n-5) -2*a(n-6) +
 *            4*a(n-7) -2*a(n-8) +4*a(n-9) -2*a(n-10) +3*a(n-11) -2*a(n-12) +
 *            a(n-13) -2*a(n-14) - a(n-16) Initial values: a(0)=1, a(1)=4,
 *            a(2)=9, a(3)=15, a(4)=17, a(5)=23, a(6)=31, a(7)=34, a(8)=38,
 *            a(9)=42, a(10)=52, a(11)=55, a(12)=55, a(13)=65, a(...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[24] = {
        1,4,9,15,17,23,31,34,38,42,52,55,55,65,70,75,76,80,95,93,93,104,111,116
    };

    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of values currently stored */

    /* For n less than stored length just return */
    if (len && n < len) return cache[n];

    /* First allocation: take room for at least 64 elements. */
    if (!cap) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – unlikely in tests */
        for (size_t i = 0; i < 24 && i < cap; ++i)
            cache[i] = init[i];
        len = 24;
    }

    /* Grow and compute new values up to n */
    while (len <= n) {
        if (len >= cap) {          /* double capacity */
            cap <<= 1;
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            if (!cache) return 0;
        }

        seq_t res;
        /* Use the initial values for indices < 24 */
        if (len < 24) {
            res = init[len];
        } else {
            long long t = 0;
            t += -2LL * (long long)cache[len-2];
            t +=  1LL * (long long)cache[len-3];
            t += -2LL * (long long)cache[len-4];
            t +=  3LL * (long long)cache[len-5];
            t += -2LL * (long long)cache[len-6];
            t +=  4LL * (long long)cache[len-7];
            t += -2LL * (long long)cache[len-8];
            t +=  4LL * (long long)cache[len-9];
            t += -2LL * (long long)cache[len-10];
            t +=  3LL * (long long)cache[len-11];
            t += -2LL * (long long)cache[len-12];
            t +=  1LL * (long long)cache[len-13];
            t += -2LL * (long long)cache[len-14];
            t += -1LL * (long long)cache[len-16];
            res = (seq_t)t;   /* final value is guaranteed non‑negative */
        }

        cache[len++] = res;
    }

    return cache[n];
}
