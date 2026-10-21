/*
 * A297476  Number of maximal matchings in the 2n-crossed prism graph.
 * https://oeis.org/A297476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 14*a(n-3) + 4*a(n-4) + 8*a(n-5) for
 *            n > 5 with initial values a(1)=5, a(2)=17, a(3)=107, a(4)=553,
 *            a(5)=2635
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t max_n = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Ensure sufficient storage for indices up to n */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    static const seq_t base[5] = {5, 17, 107, 553, 2635};

    for (idx_t m = max_n + 1; m <= n; ++m) {
        if (m <= 5) {
            cache[m - 1] = base[m - 1];
        } else {
            seq_t a1 = cache[m - 2];   /* a(m-1) */
            seq_t a2 = cache[m - 3];   /* a(m-2) */
            seq_t a3 = cache[m - 4];   /* a(m-3) */
            seq_t a4 = cache[m - 5];   /* a(m-4) */
            seq_t a5 = cache[m - 6];   /* a(m-5) */
            cache[m - 1] = 5 * a1 - 4 * a2 + 14 * a3 + 4 * a4 + 8 * a5;
        }
    }

    max_n = n;
    return cache[n - 1];
}
