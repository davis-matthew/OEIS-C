/*
 * A308076  G.f. A(x) satisfies: A(x) = x + 2*A(x^2) + 4*A(x^3) + 8*A(x^4) +
 *          ... + 2^(k-1)*A(x^k) + ...
 * https://oeis.org/A308076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} 2^(n/d-1) * a(d), with a(1)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   isqrt, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed_max = 0;

    if (n == 0) return (seq_t)0;   /* undefined, harmless */

    /* initialize cache if not already */
    if (cache == NULL) {
        cache_size = 2;                  /* indices 0 and 1 */
        cache = alloc_seq_array(cache_size);
        for (size_t i = 0; i < cache_size; ++i) cache[i] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed_max = 1;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        size_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *new_cache = alloc_seq_array(new_size);
        for (size_t i = 0; i < new_size; ++i) new_cache[i] = (seq_t)0;
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* value already known? */
    if (n <= computed_max) return cache[n];

    /* compute any missing values up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        if (k == 1) { cache[1] = (seq_t)1; continue; }

        seq_t sum = (seq_t)0;
        idx_t sqrt_k = isqrt(k);

        for (idx_t d = 1; d <= sqrt_k; ++d) {
            if (k % d != 0) continue;
            idx_t e = k / d;

            /* divisor d contributes */
            if (d < k) {
                sum += seq_mul(cache[d], pow2(k / d - 1));
            }

            /* complementary divisor e contributes if distinct */
            if (e != d && e < k) {
                sum += seq_mul(cache[e], pow2(d - 1));
            }
        }

        cache[k] = sum;
    }

    computed_max = n;
    return cache[n];
}
