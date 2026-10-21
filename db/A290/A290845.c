/*
 * A290845  a(1) = 1; a(n) = Sum_{k=1..n} a(ceiling((n-1)/k)).
 * https://oeis.org/A290845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = Sum_{k=1..n} a(ceil((n-1)/k)) for n > 1
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* array a[1..] */
static idx_t  cache_cap = 0;         /* allocated capacity (>= cache_len+1) */
static idx_t  cache_len = 0;         /* highest index computed (>=0) */

static void ensure_capacity(idx_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) exit(1);             /* allocation failure */
    if (cache) {
        memcpy(tmp, cache, sizeof(seq_t) * (cache_len + 1));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* out of bounds, but never called */
    if (n == 1) return (seq_t)1;    /* base case */

    /* fill cache up to n */
    if (cache_len < 1) {            /* first call: init base */
        ensure_capacity(2);
        cache[1] = (seq_t)1;
        cache_len = 1;
    }
    if (n > cache_len) {
        ensure_capacity(n + 1);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t m = i - 1;
            for (idx_t k = 1; k <= i; ++k) {
                idx_t j = (m + k - 1) / k;   /* ceil((i-1)/k) */
                sum += cache[j];
            }
            cache[i] = sum;
        }
        cache_len = n;
    }
    return cache[n];
}
