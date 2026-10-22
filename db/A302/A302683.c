/*
 * A302683  Number of 5Xn 0..1 arrays with every element equal to 0, 1, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 10*a(n-3) + 13*a(n-4) - 7*a(n-6) -
 *            6*a(n-7) - 11*a(n-8) - 6*a(n-9) - 4*a(n-10) - 2*a(n-11) Initial
 *            values: a(1)=16, a(2)=9, a(3)=11, a(4)=33, a(5)=63, a(6)=187,
 *            a(7)=661, a(8)=1837, a(9)=5075, a(10)=15661, a(11)=46975,
 *            a(12)=135191, a(13)=398107, a(14)=1187251, a(15)=3492...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[17] = {
    0ULL,
    16ULL, 9ULL, 11ULL, 33ULL, 63ULL, 187ULL,
    661ULL, 1837ULL, 5075ULL, 15661ULL, 46975ULL,
    135191ULL, 398107ULL, 1187251ULL, 3492697ULL, 10253381ULL
};

/* cached values for all n computed so far */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size */
static idx_t cache_max = 0;   /* highest n for which cache[*] is valid */

/* ensure cache contains at least 17 entries and the initial values are set   */
static void init_cache(void)
{
    if (cache_cap < 17) {
        if (cache) free(cache);
        cache = (seq_t *)malloc(17 * sizeof(seq_t));
        cache_cap = 17;
        for (size_t i = 1; i < 17; ++i) cache[i] = init_vals[i];
        cache_max = 16;
    }
}

seq_t a(idx_t n)
{
    /* small n -> immediate result */
    if (n <= 16) {
        init_cache();
        return cache[(size_t)n];
    }

    init_cache();

    /* enlarge cache if necessary */
    if ((idx_t)cache_cap <= n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* compute successive terms if needed */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        size_t j = (size_t)i;           /* for array indexing */

        seq_t val = cache[j - 1] + cache[j - 2];
        val += (seq_t)10 * cache[j - 3];
        val += (seq_t)13 * cache[j - 4];
        val -= (seq_t)7  * cache[j - 6];
        val -= (seq_t)6  * cache[j - 7];
        val -= (seq_t)11 * cache[j - 8];
        val -= (seq_t)6  * cache[j - 9];
        val -= (seq_t)4  * cache[j - 10];
        val -= (seq_t)2  * cache[j - 11];

        cache[j] = val;
    }

    cache_max = n;
    return cache[(size_t)n];
}
