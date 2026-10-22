/*
 * A361271  Number of 1342-avoiding odd Grassmannian permutations of size n.
 * https://oeis.org/A361271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) + 3·a(n-2) -
 *            3·a(n-3) - 3·a(n-4) + 3·a(n-5) + a(n-6) - a(n-7) for n ≥ 7, with
 *            initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=6, a(5)=9,
 *            a(6)=19. This is equivalent to the generating function
 *            x²·(x⁴+x²+x+1)/((1+x)³·(1−x)⁴)
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    if (cache_cap == 0) {
        size_t init = 7;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)9;
        cache[6] = (seq_t)19;
        cache_cap = init;
    }

    if ((size_t)n >= cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            /* compute a(i) by the recurrence */
            seq_t s1 = cache[i-1];
            s1 += cache[i-2] * (seq_t)3;
            s1 += cache[i-5] * (seq_t)3;
            s1 += cache[i-6];

            seq_t s2 = cache[i-3] * (seq_t)3;
            s2 += cache[i-4] * (seq_t)3;
            s2 += cache[i-7];

            cache[i] = s1 - s2;
        }
    }

    return cache[(size_t)n];
}
