/*
 * A257293  Numbers n such that T(n) + T(n+1) + ... + T(n+12) is a square,
 *          where T = A000217 (triangular numbers).
 * https://oeis.org/A257293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=29, a(3)=75, a(4)=432, a(5)=998, a(6)=3624,
 *            a(7)=8310, a(8)=44717, a(9)=102443; for n>=10:
 *            a(n)=a(n-1)+102*a(n-4)-102*a(n-5)-a(n-8)+a(n-9) (offset 1). This
 *            is the linear recurrence that follows from the generating
 *            function of A257293. The sequence is strictly increasing, so the
 *            inter...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   mul102
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul102.h"

static const seq_t init_terms[9] = {
    (seq_t)3,  (seq_t)29,  (seq_t)75,  (seq_t)432,
    (seq_t)998, (seq_t)3624, (seq_t)8310, (seq_t)44717,
    (seq_t)102443
};

static seq_t *cache   = NULL; /* terms [0..cache_size-1] correspond to a(1)..a(cache_size) */
static size_t cache_size = 0; /* number of terms currently in cache */
static size_t cache_cap   = 0; /* allocated capacity of cache */

/* ensure that the cache can hold at least new_cap terms */
static void ensure_cache_cap(size_t new_cap) {
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap << 1 : 16;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = cap;
}

/* initialise the cache with the nine known terms */
static void init_cache(void) {
    if (cache_size) return;
    ensure_cache_cap(16);
    for (int i = 0; i < 9; ++i) cache[i] = init_terms[i];
    cache_size = 9;
}

/* compute a(n) with the caching strategy described above */
seq_t a(idx_t n) {
    init_cache();                  /* make sure cache is initialised */

    if (n <= cache_size) return cache[n-1];

    /* extend cache to contain up to term a(n) */
    ensure_cache_cap((size_t)n);
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        size_t idx = i - 1;                /* zero‑based index of a(i) */

        /* indices of required previous terms (zero‑based) */
        size_t p_1  = idx - 1;             /* a(i-1) */
        size_t p_4  = idx - 4;             /* a(i-4) */
        size_t p_5  = idx - 5;             /* a(i-5) */
        size_t p_8  = idx - 8;             /* a(i-8) */
        size_t p_9  = idx - 9;             /* a(i-9) */

        /* a(i) = a(i-1) - a(i-8) + a(i-9) + 102*(a(i-4)-a(i-5))   */
        seq_t term = cache[p_1];
        term -= cache[p_8];
        term += cache[p_9];

        seq_t diff = cache[p_4];
        diff -= cache[p_5];
        term += mul102(diff);

        cache[idx] = term;
    }
    cache_size = n;
    return cache[n-1];
}
