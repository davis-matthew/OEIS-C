/*
 * A307856  a(1) = a(2) = 1; a(n) = Sum_{1 < k < n, k not dividing n} a(k).
 * https://oeis.org/A307856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 < k < n, k not dividing n} a(k) a(1)=a(2)=1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static seq_t prefix_sum = 0;

seq_t a(idx_t n)
{
    /* ensure base terms */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)1;          /* a(2) */
        cache_len = 2;
        prefix_sum = cache[1];        /* sum of a(2) */
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* grow capacity if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n * 2 + 10;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t m = cache_len + 1; m <= n; ++m) {
        seq_t div_sum = 0;
        for (idx_t i = 2; i <= m / i; ++i) {
            if (m % i == 0) {
                div_sum += cache[i - 1];
                idx_t j = m / i;
                if (j != i)
                    div_sum += cache[j - 1];
            }
        }
        seq_t val = prefix_sum - div_sum;
        cache[cache_len] = val;
        ++cache_len;
        prefix_sum += val;
    }

    return cache[n - 1];
}
