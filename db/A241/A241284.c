/*
 * A241284  Number of 2Xn 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or zero plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 7*a(n-3) - 8*a(n-4) - 7*a(n-5) +
 *            9*a(n-7) +10*a(n-8) - 24*a(n-9) + 5*a(n-10) +29*a(n-11) -
 *            18*a(n-12) -10*a(n-13) -10*a(n-14) +16*a(n-15) +12*a(n-16)
 *            -12*a(n-17) (n>18) with a(1)=4, a(2)=3, a(3)=5, a(4)=13,
 *            a(5)=17, a(6)=35, a(7)=90, a(8)=141, a(9)=288, a(10)=670,
 *            a(11)=1...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* 1‑based array of computed terms */
static idx_t cache_len = 0;           /* highest index currently stored */

static const seq_t init[19] = {
    0, /* unused, 1‑based */
    4, 3, 5, 13, 17, 35, 90, 141,
    288, 670, 1101, 2265, 4995,
    8474, 17477, 37329, 64894, 133967
};

static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Allocate space for indices 1 … n (index 0 unused) */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                   /* allocation failure */
    cache = new_cache;

    /* Compute new entries */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 18) {
            cache[i] = init[i];
        } else {
            seq_t pos = cache[i-1] + cache[i-2] + cache[i-3]*7
                        + cache[i-7]*9 + cache[i-8]*10 + cache[i-10]*5
                        + cache[i-11]*29 + cache[i-15]*16 + cache[i-16]*12;
            seq_t neg = cache[i-4]*8 + cache[i-5]*7 + cache[i-9]*24
                        + cache[i-12]*18 + cache[i-13]*10 + cache[i-14]*10
                        + cache[i-17]*12;
            cache[i] = pos - neg;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1, but 0 is harmless */
    ensure_cache(n);
    return cache[n];
}
