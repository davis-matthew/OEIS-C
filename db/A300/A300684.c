/*
 * A300684  Number of nX3 0..1 arrays with every element equal to 1, 2, 3 or 6
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) - 11*a(n-3) + a(n-4) - 9*a(n-5) -
 *            3*a(n-6) + 10*a(n-7) - 2*a(n-8) - 14*a(n-9) - 8*a(n-10) -
 *            4*a(n-11) + 8*a(n-13) + 4*a(n-14) Initial values: a(1)=1,
 *            a(2)=10, a(3)=53, a(4)=272, a(5)=1391, a(6)=7187, a(7)=37100,
 *            a(8)=191529, a(9)=988754, a(10)=5104580, a(11)=26353052, a...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* realloc, abort */
#include "../utils/mul_small.h"  /* mul_small */

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined region; return 0 */
        return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    static const seq_t init[15] = {
        (seq_t)1ULL,
        (seq_t)10ULL,
        (seq_t)53ULL,
        (seq_t)272ULL,
        (seq_t)1391ULL,
        (seq_t)7187ULL,
        (seq_t)37100ULL,
        (seq_t)191529ULL,
        (seq_t)988754ULL,
        (seq_t)5104580ULL,
        (seq_t)26353052ULL,
        (seq_t)136051331ULL,
        (seq_t)702383958ULL,
        (seq_t)3626156187ULL,
        (seq_t)18720542075ULL
    };

    size_t req = (size_t)n;      /* requested number of terms (n ≥ 1) */

    /* Resize cache if needed */
    if (req > cache_cap) {
        size_t new_cap = req;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        /* initialize the newly allocated part to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < req; ++i) {
        if (i < 15) {
            cache[i] = init[i];
        } else {
            seq_t res = (seq_t)0;

            /* Positive terms */
            res += mul_small(cache[i - 1], 5u);
            res += mul_small(cache[i - 2], 3u);
            res += cache[i - 4];                     /* coefficient 1 */
            res += mul_small(cache[i - 7], 10u);
            res += mul_small(cache[i - 13], 8u);
            res += mul_small(cache[i - 14], 4u);

            /* Negative terms */
            res -= mul_small(cache[i - 3], 11u);
            res -= mul_small(cache[i - 5], 9u);
            res -= mul_small(cache[i - 6], 3u);
            res -= mul_small(cache[i - 8], 2u);
            res -= mul_small(cache[i - 9], 14u);
            res -= mul_small(cache[i - 10], 8u);
            res -= mul_small(cache[i - 11], 4u);

            cache[i] = res;
        }
    }
    cache_len = req;

    return cache[req - 1];
}
