/*
 * A268954  Number of length-n 0..6 arrays with no repeated value equal to the
 *          previous repeated value, with new values introduced in sequential
 *          order.
 * https://oeis.org/A268954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) -280*a(n-2) +1258*a(n-3) -781*a(n-4)
 *            -13833*a(n-5) +39878*a(n-6) +32104*a(n-7) -261136*a(n-8)
 *            +111564*a(n-9) +754544*a(n-10) -663920*a(n-11) -1052352*a(n-12)
 *            +1135872*a(n-13) +580608*a(n-14) -663552*a(n-15) for n>17
 *            (a(1)=1, a(2)=2, ..., a(17)=32923518967)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Base values up to n = 17 */
    static const seq_t base[18] = {
        0ull,                           /* index 0 unused */
        1ull, 2ull, 4ull, 12ull, 40ull, 154ull, 656ull,
        3073ull, 15579ull, 84572ull, 485726ull,
        2918103ull, 18146537ull, 115772922ull, 752410412ull,
        4954432229ULL, 32923518967ULL
    };

    if (n <= 17u)
        return base[(idx_t)n];

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of items stored, index 0 unused */

    /* Ensure cache contains base values up to 17 */
    if (cache == NULL) {
        cache_size = 18;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = base[i];
    }

    /* Grow cache up to n, computing new terms with recurrence */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;

            pos += mul_small(cache[(size_t)(i - 1)], 27);
            pos += mul_small(cache[(size_t)(i - 3)], 1258);
            pos += mul_small(cache[(size_t)(i - 6)], 39878);
            pos += mul_small(cache[(size_t)(i - 7)], 32104);
            pos += mul_small(cache[(size_t)(i - 9)], 111564);
            pos += mul_small(cache[(size_t)(i -10)], 754544);
            pos += mul_small(cache[(size_t)(i -13)], 1135872);
            pos += mul_small(cache[(size_t)(i -14)], 580608);

            neg += mul_small(cache[(size_t)(i - 2)], 280);
            neg += mul_small(cache[(size_t)(i - 4)], 781);
            neg += mul_small(cache[(size_t)(i - 5)], 13833);
            neg += mul_small(cache[(size_t)(i - 8)], 261136);
            neg += mul_small(cache[(size_t)(i -11)], 663920);
            neg += mul_small(cache[(size_t)(i -12)], 1052352);
            neg += mul_small(cache[(size_t)(i -15)], 663552);

            cache[i] = pos - neg;
        }

        cache_size = new_size;
    }

    return cache[(idx_t)n];
}
