/*
 * A305588  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 5 or 6 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - 3*a(n-3) - 29*a(n-4) - 14*a(n-5) * +
 *            22*a(n-6) + 95*a(n-7) + 40*a(n-8) * - 70*a(n-9) - 84*a(n-10) +
 *            13*a(n-11) + 57*a(n-12) * + 4*a(n-13) - 28*a(n-14) - 26*a(n-15)
 *            - 8*a(n-16) * for n > 17, with initial values * a(1)=4, a(2)=23,
 *            a(3)=95, a(4)=390, a(5)=1626, a(6)=6818,...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* initial values for 1 ≤ n ≤ 17 */
static const seq_t init_vals[18] = {
    0ULL, /* dummy for index 0 */
    4ULL, 23ULL, 95ULL, 390ULL, 1626ULL, 6818ULL, 28530ULL,
    119266ULL, 498890ULL, 2087148ULL, 8731184ULL, 36525814ULL,
    152804412ULL, 639253506ULL, 2674304822ULL, 11187927898ULL,
    46804640008ULL
};

/* coefficients of the recurrence, ordered by a(n-1) … a(n-16) */
static const int coeffs[16] = {
    5, -1, -3, -29, -14, 22, 95, 40,
    -70, -84, 13, 57, 4, -28, -26, -8
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    if (n <= cache_max)
        return cache[n];
    if (cache_max == 0)
        cache_max = 0;            /* cache[0] is unused but already zero */

    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i <= 17)
        {
            cache[i] = init_vals[i];
        }
        else
        {
            seq_t val = 0;
            for (int k = 0; k < 16; ++k)
            {
                seq_t term = cache[i - 1 - k];
                if (coeffs[k] >= 0)
                    val += (seq_t)coeffs[k] * term;
                else
                    val -= (seq_t)(-coeffs[k]) * term;
            }
            cache[i] = val;
        }
    }
    cache_max = n;
    return cache[n];
}
