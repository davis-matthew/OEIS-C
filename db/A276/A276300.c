/*
 * A276300  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,1) or (0,-1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A276300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) for n>4, a(1)=5, a(2)=11,
 *            a(3)=45, a(4)=173
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_n = 0;

static void ensure_cache_size(idx_t n) {
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap*2 : 1;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    /* zero remaining entries */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

static void compute_up_to(idx_t n) {
    if (n <= last_n) return;
    ensure_cache_size(n);
    for (idx_t i = last_n + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);  /* zero‑based index */
        if (i == 1) cache[idx] = 5;
        else if (i == 2) cache[idx] = 11;
        else if (i == 3) cache[idx] = 45;
        else if (i == 4) cache[idx] = 173;
        else {
            seq_t a1 = cache[idx-1];
            seq_t a2 = cache[idx-2];
            seq_t a3 = cache[idx-3];
            cache[idx] = 4*a1 + a2 - 4*a3;
        }
    }
    last_n = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;         /* safety for offset */
    compute_up_to(n);
    return cache[(size_t)(n-1)];
}
