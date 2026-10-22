/*
 * A302821  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -9*a(n-2) +2*a(n-4) -18*a(n-5) +37*a(n-6)
 *            +34*a(n-7) +5*a(n-8) -52*a(n-9) -118*a(n-10) -24*a(n-11)
 *            +190*a(n-12) +10*a(n-13) -68*a(n-14) +12*a(n-15) for n > 16
 *            Initial values: a(1)=4, a(2)=25, a(3)=75, a(4)=220, a(5)=731,
 *            a(6)=2419, a(7)=7900, a(8)=25645, a(9)=83685, a(10)=273504,...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   _mult_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_mult_loop.h"

static seq_t *cache   = NULL;    /* cache[0] unused, cache[i] = a(i) */
static size_t cache_cap = 0;     /* allocated capacity (excluding index 0) */
static size_t cache_size = 0;    /* largest index presently stored */

static void _ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) return;              /* if allocation fails, leave cache unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

static void _init_cache(void)
{
    if (cache) return;                 /* already initialised */

    static const seq_t init_vals[16] = {
        4, 25, 75, 220, 731, 2419, 7900, 25645,
        83685, 273504, 893195, 2915411,
        9518108, 31079021, 101478805, 331335348
    };

    _ensure_capacity(16);
    for (size_t i = 1; i <= 16; ++i) cache[i] = init_vals[i - 1];
    cache_size = 16;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n < 1, return 0 */

    _init_cache();
    size_t nn = (size_t)n;

    if (nn > cache_size) {
        _ensure_capacity(nn);
        for (size_t i = cache_size + 1; i <= nn; ++i) {
            seq_t val = 0;
            val += _mult_loop(cache[i - 1], 6);
            val -= _mult_loop(cache[i - 2], 9);
            val += _mult_loop(cache[i - 4], 2);
            val -= _mult_loop(cache[i - 5], 18);
            val += _mult_loop(cache[i - 6], 37);
            val += _mult_loop(cache[i - 7], 34);
            val += _mult_loop(cache[i - 8], 5);
            val -= _mult_loop(cache[i - 9], 52);
            val -= _mult_loop(cache[i -10], 118);
            val -= _mult_loop(cache[i -11], 24);
            val += _mult_loop(cache[i -12], 190);
            val += _mult_loop(cache[i -13], 10);
            val -= _mult_loop(cache[i -14], 68);
            val += _mult_loop(cache[i -15], 12);
            cache[i] = val;
        }
        cache_size = nn;
    }

    return cache[nn];
}
