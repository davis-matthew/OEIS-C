/*
 * A298805  Growth series for group with presentation < S, T : S^2 = T^3 =
 *          (S*T)^7 = 1 >.
 * https://oeis.org/A298805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + a(n-4) + 2*a(n-5) + a(n-6) + a(n-8) + a(n-10) *
 *            + 2*a(n-11) + a(n-12) - a(n-15) - a(n-16) * Initial values
 *            a(0)=1, a(1)=3, a(2)=4, a(3)=6, a(4)=8, a(5)=12, * a(6)=16,
 *            a(7)=22, a(8)=24, a(9)=34, a(10)=40, a(11)=56, a(12)=62, *
 *            a(13)=83, a(14)=98, a(15)=133, a(16)=152, a(17)=202, a...
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 93, u64 n <= 193,
 *            u128 n <= 394, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t init[19] = {
    1, 3, 4, 6, 8, 12, 16, 22, 24, 34,
    40, 56, 62, 83, 98, 133, 152, 202, 236
};

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity of cache array  */
static size_t cache_size = 0;   /* number of values currently cached    */

seq_t a(idx_t n)
{
    /* Convert index to a type that can be compared with size_t */
    const size_t idx = (size_t)n;
    const size_t needed = idx + 1;

    /* Grow the cache array if required */
    if (needed > cache_cap) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {          /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (needed > cache_size) {
        size_t i;
        for (i = cache_size; i <= idx; ++i) {
            if (i < 19) {
                cache[i] = init[i];
            } else {
                /* Apply the recurrence for i >= 19 */
                seq_t val =  cache[i - 4]
                           + 2 * cache[i - 5]
                           +  cache[i - 6]
                           +  cache[i - 8]
                           +  cache[i -10]
                           + 2 * cache[i -11]
                           +  cache[i -12]
                           -  cache[i - 1]
                           -  cache[i -15]
                           -  cache[i -16];
                cache[i] = val;
            }
        }
        cache_size = needed;
    }

    return cache[idx];
}
