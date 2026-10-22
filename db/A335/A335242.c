/*
 * A335242  a(n) = 2*a(n-1) + a(n-3) for n >= 4, with initial values a(0) = 1,
 *          a(1) = 0, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A335242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) for n >= 4 * Initial values: a(0)=1,
 *            a(1)=0, a(2)=2, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(size_t req) {
    if (req <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < req) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

static void compute_up_to(idx_t n) {
    if (n < cache_len) return;
    ensure_capacity((size_t)n + 1);
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v;
        if (i == 0)        v = 1;
        else if (i == 1)   v = 0;
        else if (i == 2)   v = 2;
        else if (i == 3)   v = 3;
        else              v = cache[i-1] * ((seq_t)2) + cache[i-3];
        cache[i] = v;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n) {
    compute_up_to(n);
    return cache[n];
}
