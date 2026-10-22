/*
 * A325690  Number of length-3 integer partitions of n whose largest part is
 *          not the sum of the other two.
 * https://oeis.org/A325690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) * - a(n-5) - a(n-6) - a(n-7) +
 *            a(n-9) for n > 8 * initial values: * a(0)=0, a(1)=0, a(2)=0,
 *            a(3)=1, * a(4)=0, a(5)=2, a(6)=2, a(7)=4, a(8)=3
 * Range:     u8 n <= 56, u16 n <= 886, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms      */
static size_t cache_len = 0;     /* number of initialized entries */
static size_t cache_cap = 0;     /* allocated capacity           */

static const seq_t init_vals[9] = {0,0,0,1,0,2,2,4,3};

static void ensure_cache(idx_t n)
{
    /* Expand capacity if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        if (cache == NULL) {
            cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
            if (cache == NULL) exit(EXIT_FAILURE);
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(EXIT_FAILURE);
            cache = tmp;
        }
        cache_cap = new_cap;

        /* zero new slots (calloc would already zero them when allocating new memory) */
        for (size_t i = cache_len; i < cache_cap; ++i) cache[i] = 0;
    }

    /* Initialise first nine values if not yet done */
    if (cache_len < 9) {
        size_t start = cache_len;
        for (size_t i = start; i < 9; ++i) cache[i] = init_vals[i];
        cache_len = (cache_cap > 9 ? 9 : cache_cap);
    }

    /* Compute remaining values up to n */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        if (k <= 8) {
            /* already set by init_vals */
            continue;
        }
        seq_t val = 0;
        val += cache[k - 2];
        val += cache[k - 3];
        val += cache[k - 4];
        val -= cache[k - 5];
        val -= cache[k - 6];
        val -= cache[k - 7];
        val += cache[k - 9];
        cache[k] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
