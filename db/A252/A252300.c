/*
 * A252300  Number of (n+2)X(3+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-3) - 14*a(n-6) + 8*a(n-9) for n > 18. Base values
 *            for n = 1 … 18 are given explicitly below
 * Range:     u8 none, u16 n <= 11, u32 n <= 35, u64 n <= 83, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[19] = {
    0ULL,          /* dummy for index 0 */
    1675ULL, 1370ULL, 1677ULL, 2151ULL, 3669ULL, 5416ULL,
    7492ULL, 13386ULL, 20379ULL, 28688ULL, 51938ULL, 79730ULL,
    112972ULL, 205370ULL, 316178ULL, 449108ULL, 817514ULL, 1260050ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    static seq_t *cache = NULL;     /* memoization array, includes index 0 */
    static size_t cap   = 0;         /* allocated size (include index 0)   */
    static size_t last  = 0;         /* highest index computed so far     */

    size_t target = (size_t)n;       /* n fits comfortably in size_t for n up to 10000 */

    /* Grow the cache if it is not large enough. */
    if (target > cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all missing terms up to target. */
    for (size_t i = last + 1; i <= target; ++i) {
        if (i <= 18) {
            cache[i] = init_vals[i];
        } else {
            seq_t res = cache[i - 3];
            res *= 7;                /* 7*a(i-3)                  */
            seq_t term9 = cache[i - 9];
            term9 *= 8;               /* 8*a(i-9)                  */
            res += term9;             /* +8*a(i-9)                 */
            seq_t term6 = cache[i - 6];
            term6 *= 14;              /* 14*a(i-6)                 */
            res -= term6;             /* -14*a(i-6)                */
            cache[i] = res;
        }
    }
    last = target;
    return cache[target];
}
