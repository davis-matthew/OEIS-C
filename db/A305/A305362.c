/*
 * A305362  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +5*a(n-2) +2*a(n-3) -16*a(n-4) -14*a(n-5)
 *            -27*a(n-7) +32*a(n-8) +52*a(n-9) +43*a(n-10) +27*a(n-11)
 *            -34*a(n-12) -25*a(n-13) -12*a(n-14) +13*a(n-15) +a(n-16)
 *            -20*a(n-17) -8*a(n-18) initial terms: a(1)=1, a(2)=16, a(3)=39,
 *            a(4)=107, a(5)=310, a(6)=943, a(7)=2935, a(8)=9077, a(9)=2805...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = 0;        /* highest n for which recurrence was applied */

    /* base values for 1..18 */
    static const seq_t base_vals[19] = {
        0ULL,                          /* placeholder for index 0 */
        1ULL, 16ULL, 39ULL, 107ULL,
        310ULL, 943ULL, 2935ULL, 9077ULL,
        28054ULL, 87562ULL, 272571ULL,
        850365ULL, 2654831ULL, 8284969ULL,
        25870031ULL, 80782480ULL,
        252246425ULL, 787748512ULL
    };

    /* ensure enough storage */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_size) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_size = needed;
    }

    /* initialise base indices if missing */
    for (size_t i = 1; i <= 18 && i < cache_size; ++i)
        cache[i] = base_vals[i];

    /* mark base values as computed */
    if (max_computed < 18) max_computed = 18;

    /* compute recurrence for missing terms */
    if (n > max_computed) {
        for (size_t k = (size_t)(max_computed + 1); k <= (size_t)n; ++k) {
            seq_t pos = cache[k-1] * 2;
            pos += cache[k-2] * 5;
            pos += cache[k-3] * 2;
            pos += cache[k-8] * 32;
            pos += cache[k-9] * 52;
            pos += cache[k-10] * 43;
            pos += cache[k-11] * 27;
            pos += cache[k-15] * 13;
            pos += cache[k-16] * 1;

            seq_t neg = cache[k-4] * 16;
            neg += cache[k-5] * 14;
            neg += cache[k-7] * 27;
            neg += cache[k-12] * 34;
            neg += cache[k-13] * 25;
            neg += cache[k-14] * 12;
            neg += cache[k-17] * 20;
            neg += cache[k-18] * 8;

            cache[k] = pos - neg;
        }
        max_computed = n;
    }

    return cache[n];
}
