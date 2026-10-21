/*
 * A269189  Number of nX3 0..3 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling three no more than once.
 * https://oeis.org/A269189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) -131*a(n-2) +88*a(n-3) +221*a(n-4) -154*a(n-5)
 *            -101*a(n-6) +44*a(n-7) -4*a(n-8) (n>9) Initial values (for
 *            n=1..9): 60, 708, 9284, 115452, 1399612, 16629436, 194596516,
 *            2249848972, 25758552060 This code memoises the sequence; it is
 *            valid C11 and C++17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#if defined(__cplusplus)
#  include <type_traits>
#endif

static inline seq_t recurrence(idx_t n, const seq_t *cache) {
#if defined(__cplusplus)
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 sum = 0;
        sum += (__int128)cache[n-1] * 22;
        sum += (__int128)cache[n-3] * 88;
        sum += (__int128)cache[n-4] * 221;
        sum += (__int128)cache[n-7] * 44;
        sum -= (__int128)cache[n-2] * 131;
        sum -= (__int128)cache[n-5] * 154;
        sum -= (__int128)cache[n-6] * 101;
        sum -= (__int128)cache[n-8] * 4;
        return (seq_t)sum;
    } else {
        seq_t sum =   cache[n-1] * 22
                    + cache[n-3] * 88
                    + cache[n-4] * 221
                    + cache[n-7] * 44
                    - cache[n-2] * 131
                    - cache[n-5] * 154
                    - cache[n-6] * 101
                    - cache[n-8] * 4;
        return sum;
    }
#else
    /* In C, seq_t is an unsigned integral type. Use 128‑bit intermediates. */
    __int128 sum = 0;
    sum += (__int128)cache[n-1] * 22;
    sum += (__int128)cache[n-3] * 88;
    sum += (__int128)cache[n-4] * 221;
    sum += (__int128)cache[n-7] * 44;
    sum -= (__int128)cache[n-2] * 131;
    sum -= (__int128)cache[n-5] * 154;
    sum -= (__int128)cache[n-6] * 101;
    sum -= (__int128)cache[n-8] * 4;
    return (seq_t)sum;
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (n == 0)
        return 0;           /* offset starts at 1, so skip index 0 */

    /* grow cache when needed */
    if (size <= n) {
        idx_t newsize = size ? size * 2 : 256;
        while (newsize <= n) newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + size, 0, (newsize - size) * sizeof(seq_t));
        cache = tmp;
        size = newsize;
    }

    /* initialise the first nine terms (indices 1..9) */
    if (cache[1] == 0) {
        static const seq_t init[10] = { 0,
            60ULL, 708ULL, 9284ULL, 115452ULL,
            1399612ULL, 16629436ULL, 194596516ULL,
            2249848972ULL, 25758552060ULL };
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
    }

    /* compute all terms up to n */
    for (idx_t i = 10; i <= n; ++i) {
        if (cache[i] == 0)
            cache[i] = recurrence(i, cache);
    }

    return cache[n];
}
