/*
 * A298141  Number of n X 3 0..1 arrays with every element equal to 2, 3, 6 or
 *          7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-5) - 4*a(n-6) - 8*a(n-7) -
 *            a(n-8) + a(n-9) Initial values (offset 1): a(1)=0, a(2)=2,
 *            a(3)=0, a(4)=2, a(5)=1, a(6)=6, a(7)=8, a(8)=15, a(9)=22
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 75, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* indexed by n (cache[0] unused) */
static size_t cache_len = 0;      /* largest n computed so far */
static size_t cache_cap = 0;      /* allocated capacity */

/* Ensure the cache can hold values up to n (inclusive). */
static void ensure_cache(idx_t n) {
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n<1; return 0 as fallback */

    ensure_cache(n);

    if (n <= cache_len) return cache[n];

    /* Persistent base values for 1 ≤ n ≤ 9 */
    static const seq_t base[10] = {0, 0, 2, 0, 2, 1, 6, 8, 15, 22};

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = base[i];
        } else {
            seq_t val = cache[i - 1]
                      + 2 * cache[i - 2]
                      + 2 * cache[i - 5]
                      - 4 * cache[i - 6]
                      - 8 * cache[i - 7]
                      - cache[i - 8]
                      + cache[i - 9];
            cache[i] = val;
        }
    }
    cache_len = n;
    return cache[n];
}
