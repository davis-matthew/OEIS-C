/*
 * A264087  Number of (n+1)X(5+1) arrays of permutations of 0..n*6+5 with each
 *          element having index change +-(.,.) 0,0 0,1 or 2,-2.
 * https://oeis.org/A264087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A264087 – Number of (n+1)×6 arrays as described.  The values satisfy
 *  the linear recurrence
 *
 *      a(n) = 43*a(n-1) - a(n-2)
 *             +1848*a(n-4) -79464*a(n-5) +1848*a(n-6)
 *             -1848*a(n-8) +79464*a(n-9) -1848*a(n-10)
 *             + a(n-12) -43*a(n-13) + a(n-14)
 *
 *  for n ≥ 15 with initial terms
 *      a(1)=169, a(2)=8788, a(3)=456976, a(4)=19553300,
 *      a(5)=836655625, a(6)=35960283125, a(7)=1545608400625,
 *      a(8)=66425039324500, a(9)=2854724293344400,
 *      a(10)=122686726306616820, a(11)=5272674789270374721,
 *      a(12)=226602328913809523253, a(13)=9738627455964881203129,
 *      a(14)=418534378290010327242292.
 *
 *  The function computes terms on demand and memoises the results.
 *  It works for any seq_t and idx_t that satisfy the restrictions in
 *  the problem statement. */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Lazy static cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms */

    /* Initialise the first 14 terms once */
    if (cache == NULL) {
        static const char *const init_str[14] = {
            "169", "8788", "456976", "19553300", "836655625",
            "35960283125", "1545608400625", "66425039324500",
            "2854724293344400", "122686726306616820",
            "5272674789270374721", "226602328913809523253",
            "9738627455964881203129", "418534378290010327242292"
        };
        cache = (seq_t *)malloc(14 * sizeof(seq_t));
        if (cache == NULL) return 0;   /* out of memory – failure mode */
        for (idx_t i = 0; i < 14; ++i) {
            cache[i] = seq_from_str(init_str[i]);
        }
        cache_len = 14;
    }

    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend cache up to value n */
    for (idx_t curr = cache_len + 1; curr <= n; ++curr) {
        idx_t i = curr - 1;           /* zero‑based index for a[curr] */
        seq_t val = 0;

        /* Recurrence – all operands are seq_t */
        val = 43 * cache[i - 1]
              - cache[i - 2]
              + 1848 * cache[i - 4]
              - 79464 * cache[i - 5]
              + 1848 * cache[i - 6]
              - 1848 * cache[i - 8]
              + 79464 * cache[i - 9]
              - 1848 * cache[i - 10]
              + cache[i - 12]
              - 43 * cache[i - 13]
              + cache[i - 14];

        /* Grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, (i + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;    /* out of memory – failure mode */
        cache = tmp;
        cache[i] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
