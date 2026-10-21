/*
 * A271036  Number of 4 X 4 X 4 triangular 0..n arrays with some element less
 *          than a w, nw or ne neighbor exactly once.
 * https://oeis.org/A271036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 55*a(n-2) + 165*a(n-3) - 330*a(n-4) * +
 *            462*a(n-5) - 462*a(n-6) + 330*a(n-7) - 165*a(n-8) * + 55*a(n-9)
 *            - 11*a(n-10) + a(n-11) for n>11 * with base values: * a(1)=34
 *            a(2)=450 a(3)=3114 a(4)=15116 a(5)=58036 * a(6)=188034
 *            a(7)=535106 a(8)=1373944 a(9)=3244098 * a(10)=7143708 a(1...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[1] = a(1)... */
static idx_t  cache_len = 0;         /* allocated size (elements 0..cache_len-1) */
static idx_t  cached_computed_max = 0; /* largest n already computed */

static const seq_t base_vals[12] = {
    0ULL,                  /* dummy for index 0 */
    34ULL, 450ULL, 3114ULL, 15116ULL, 58036ULL,
    188034ULL, 535106ULL, 1373944ULL, 3244098ULL,
    7143708ULL, 14829100ULL
};

static void ensure_cache(idx_t n)
{
    /* Ensure allocated size can hold indices up to n */
    if ((size_t)(n + 1) <= cache_len)
        return;

    size_t old_len = cache_len;
    size_t new_len = (size_t)(n + 1);

    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed */
        exit(1);
    }
    cache = new_cache;
    cache_len = new_len;

    /* Initialize newly added entries */
    for (idx_t i = old_len; i < new_len; ++i) {
        if (i <= 11) {
            cache[i] = base_vals[i];
        } else {
            cache[i] = 0;   /* will be computed later */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined below offset */

    ensure_cache(n);

    if (n <= cached_computed_max)
        return cache[n];

    /* Compute terms from the first missing index up to n */
    idx_t start = cached_computed_max + 1;
    if (start < 12)
        start = 12;             /* recurrence applies only for n>11 */

    for (idx_t i = start; i <= n; ++i) {
        cache[i] =
            (seq_t)11  * cache[i-1]
          - (seq_t)55  * cache[i-2]
          + (seq_t)165 * cache[i-3]
          - (seq_t)330 * cache[i-4]
          + (seq_t)462 * cache[i-5]
          - (seq_t)462 * cache[i-6]
          + (seq_t)330 * cache[i-7]
          - (seq_t)165 * cache[i-8]
          + (seq_t)55  * cache[i-9]
          - (seq_t)11  * cache[i-10]
          +        cache[i-11];
    }

    cached_computed_max = n;
    return cache[n];
}
