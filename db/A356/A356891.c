/*
 * A356891  a(n) = a(n-1) * a(n-2) + 1 if n is even, otherwise a(n) = a(n-3) +
 *          1, with a(0) = a(1) = 1.
 * https://oeis.org/A356891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * a(n-2) + 1 for even n, a(n) = a(n-3) + 1 for odd
 *            n, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 15, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_cache(idx_t n, seq_t **buf, idx_t *used, idx_t *cap) {
    if (n + 1 <= *cap) return;

    idx_t new_cap = (*cap) ? (*cap) * 2 : 16;
    while (new_cap < n + 1) new_cap *= 2;

#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];
#else
    seq_t *tmp = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
#endif
    if (!tmp) exit(1);

    /* copy already computed values */
    for (idx_t j = 0; j < *used; ++j) tmp[j] = (*buf)[j];

    /* free old storage */
#if defined(__cplusplus)
    delete[] *buf;
#else
    free(*buf);
#endif

    *buf  = tmp;
    *cap  = new_cap;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t used = 0;
    static idx_t cap = 0;

    if (n < used) return cache[n];

    ensure_cache(n, &cache, &used, &cap);

    for (idx_t i = used; i <= n; ++i) {
        seq_t val;
        if (i == 0 || i == 1) {
            val = 1;
        } else if ((i & 1) == 0) {          /* even n */
            val = cache[i - 1] * cache[i - 2] + 1;
        } else {                             /* odd n */
            val = cache[i - 3] + 1;
        }
        cache[i] = val;
    }
    used = n + 1;
    return cache[n];
}
