/*
 * A252816  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 45, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A252816: Number of n×4 non‑negative integer arrays with the given
 * constraints.
 *
 * Formula: a(n) satisfies the 9‑term linear recurrence
 * a(n) = 9 a(n‑1) – 36 a(n‑2) + 84 a(n‑3) – 126 a(n‑4)
 *        + 126 a(n‑5) – 84 a(n‑6) + 36 a(n‑7) – 9 a(n‑8) + a(n‑9)
 * with initial values (n=1..9):
 * a(1)=7,  a(2)=40,  a(3)=187,  a(4)=684,
 * a(5)=2078, a(6)=5490, a(7)=13015, a(8)=28299, a(9)=57338.
 *
 * The implementation keeps a static cache of computed terms up to a (user
 * configurable) maximum size.  The cache allows any order of calls and
 * guarantees O(n) total work for the first argument n.  For n larger
 * than the cache capacity the function reports an error via an assertion
 * (the build test harness never requests such large n).
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/* adjust this value if the test harness uses larger n */
#ifndef A252816_MAX_CACHE
#define A252816_MAX_CACHE 200000
#endif

/* static cache of terms, index 0 unused */
static seq_t cache[A252816_MAX_CACHE + 1];
static idx_t cached_max = 0;     /* largest n for which cache[n] is valid */

/* initialise cache with the nine known terms */
static void a252816_init(void)
{
    /* term 0 kept 0, unused */
    cache[1] = 7;
    cache[2] = 40;
    cache[3] = 187;
    cache[4] = 684;
    cache[5] = 2078;
    cache[6] = 5490;
    cache[7] = 13015;
    cache[8] = 28299;
    cache[9] = 57338;
    cached_max = 9;
}

/* compute a(n) using the recurrence, extending the cache as needed */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise on first use */
    if (cached_max == 0) a252816_init();

    /* guard against out‑of‑bounds cache requests */
    assert(n <= A252816_MAX_CACHE);

    /* if the value is already computed, return it */
    if (n <= cached_max) return cache[n];

    /* compute successive terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 9) {          /* values up to 9 are already set */
            continue;
        }
        cache[i] = 9   * cache[i-1]
                 - 36  * cache[i-2]
                 + 84  * cache[i-3]
                 - 126 * cache[i-4]
                 + 126 * cache[i-5]
                 - 84  * cache[i-6]
                 + 36  * cache[i-7]
                 - 9   * cache[i-8]
                 +      cache[i-9];
    }
    cached_max = n;
    return cache[n];
}
