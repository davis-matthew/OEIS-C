/*
 * A361017  Dirichlet inverse of Thue-Morse sequence, A010060.
 * https://oeis.org/A361017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9066, i16 n <= 9066, i32 n <= 9066, i64 n <= 9066,
 *            i128 n <= 9066, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet inverse of Thue-Morse sequence (A361017):
   a(1) = 1; for n > 1,
   a(n) = - Σ_{d|n, d<n} t(n/d) * a(d),
   where t(k) = parity of popcount(k). */
#include <stdint.h>
#include <stdlib.h>

static inline int parity64(uint64_t x) {
    return (__builtin_popcountll(x) & 1);
}

static seq_t *cache = NULL;
static size_t cache_size = 0;  /* highest index that has been computed */

static void ensure_cache_to(idx_t n) {
    if (n <= cache_size) return;

    size_t target = (size_t)n + 1;           /* need indices 0 .. n inclusive */
    seq_t *new_cache = (seq_t*)realloc(cache, target * sizeof(seq_t));
    if (!new_cache) abort();

    cache = new_cache;

    if (cache_size == 0) {
        cache[0] = 0;          /* unused placeholder */
        cache[1] = 1;          /* a(1) = 1 */
        cache_size = 1;
    }

    /* compute a(i) for i = cache_size+1 .. n */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        size_t ii = i;
        for (size_t d = 1; (uint64_t)d * d <= ii; ++d) {
            if (ii % d != 0) continue;
            size_t d1 = d;
            size_t d2 = ii / d;

            if (d1 < ii && parity64((uint64_t)d2)) {
                sum += cache[d1];
            }
            if (d2 != d1 && d2 < ii && parity64((uint64_t)d1)) {
                sum += cache[d2];
            }
        }
        cache[i] = -sum;
    }
    cache_size = (size_t)n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cache_to(n);
    return cache[(size_t)n];
}
