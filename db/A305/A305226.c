/*
 * A305226  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3) - a(n-4) - 3*a(n-5) - 2*a(n-6)
 *            + 2*a(n-7) for n > 9 Initial values: a(1)=8, a(2)=49, a(3)=54,
 *            a(4)=116, a(5)=253, a(6)=541, a(7)=1248, a(8)=2831, a(9)=6471
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 51, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached sequence values, 1-based index */
static idx_t cached_max = 0;    /* largest n cached so far */

static const seq_t init[10] = {
    0ULL,    /* unused: index 0 */
    8ULL,    49ULL, 54ULL, 116ULL, 253ULL,
    541ULL, 1248ULL, 2831ULL, 6471ULL
};

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices – the offset is 1 and behaviour
       below that is irrelevant. */
    if (n == 0) return 0;

    /* Allocate and initialise the cache on the first call. */
    if (cache == NULL) {
        /* Ensure the cache has at least space for indices 1..9. */
        idx_t cap = (n + 1 > 10 ? n + 1 : 10);
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)cap);
        if (!cache) return 0;          /* allocation failure: abort */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        /* Zero remaining entries up to the allocated capacity (optional). */
        for (idx_t i = 10; i < (idx_t)cap; ++i)
            cache[i] = 0;
        cached_max = 9;
    }

    /* If the requested term is already cached, return it. */
    if (n <= cached_max)
        return cache[n];

    /* Resize the cache to accommodate up to n. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > (size_t)(cached_max + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;           /* allocation failure: abort */
        cache = tmp;
    }

    /* Compute all new terms up to n using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 9) {
            val = init[i];
        } else {
            val = cache[i - 1];
            val += (seq_t)3 * cache[i - 2];
            val += cache[i - 3];
            val -= cache[i - 4];
            val -= (seq_t)3 * cache[i - 5];
            val -= (seq_t)2 * cache[i - 6];
            val += (seq_t)2 * cache[i - 7];
        }
        cache[i] = val;
    }

    cached_max = n;
    return cache[n];
}
