/*
 * A290924  a(n) = (1/2)*A290923(n).
 * https://oeis.org/A290924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) + 6*a(n-3) - a(n-4) a(0) = 1, a(1) =
 *            5, a(2) = 23, a(3) = 104
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static size_t len = 0;

static void init_cache(void) {
    if (cache != NULL)
        return;
    cap = 8;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache)
        abort();
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)5;
    cache[2] = (seq_t)23;
    cache[3] = (seq_t)104;
    len = 4;
}

static void ensure_capacity(size_t needed) {
    if (needed <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 8;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cap = newcap;
}

seq_t a(idx_t n) {
    init_cache();
    if ((idx_t)len > n)
        return cache[n];

    size_t needed = (size_t)n + 1;
    ensure_capacity(needed);

    idx_t start = (idx_t)len;
    for (idx_t i = start; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t t1 = (seq_t)6 * a1;
        seq_t t2 = (seq_t)8 * a2;
        seq_t t3 = (seq_t)6 * a3;
        seq_t val = t1 - t2 + t3 - a4;
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[n];
}
