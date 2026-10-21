/*
 * A247649  Number of terms in expansion of f^n mod 2, where f = 1/x^2 + 1/x +
 *          1 + x + x^2 mod 2.
 * https://oeis.org/A247649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd coefficients in (1/x^2 + 1/x + 1 + x +
 *            x^2)^n (mod 2)
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   xor_shift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/xor_shift.h"

/* Static cache of already computed values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;          /* number of slots allocated */
static idx_t computed_max = 0;        /* highest n that has been computed */

/* Bitsets for the current and next polynomial (mod 2 coefficients) */
static uint64_t *bits_curr = NULL;
static uint64_t *bits_next = NULL;
static size_t bits_cap = 0;           /* capacity in 64‑bit words */

/* Ensure that all terms up to n have been computed. */
static void compute_to(idx_t n)
{
    if (n < computed_max) return;   /* already available */

    /* Determine word sizes needed for exponents [-2n,2n] */
    size_t bits_needed = 4 * n + 1;
    size_t words_needed = (bits_needed + 63) >> 6;

    /* Allocate or grow bitsets */
    if (bits_cap < words_needed) {
        uint64_t *tmp1 = (uint64_t*)realloc(bits_curr, words_needed * sizeof(uint64_t));
        uint64_t *tmp2 = (uint64_t*)realloc(bits_next, words_needed * sizeof(uint64_t));
        if (!tmp1 || !tmp2) exit(EXIT_FAILURE);
        bits_curr = tmp1;
        bits_next = tmp2;
        bits_cap = words_needed;
    }

    /* Allocate or grow cache */
    if ((idx_t)(n + 1) > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = n + 1;
    }

    /* Initialisation (f^0 = 1).  Do this only once. */
    if (computed_max == 0 && n >= 0) {
        memset(bits_curr, 0, bits_cap * sizeof(uint64_t));
        bits_curr[0] = 1ULL;   /* exponent 0 corresponds to bit 0 */
        cache[0] = 1ULL;       /* a(0) = 1 */
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        /* Length of source array for f^(k-1) */
        size_t src_len = (4 * (k - 1) + 1 + 63) >> 6;
        /* Length of destination array for f^k */
        size_t dst_len = (4 * k + 1 + 63) >> 6;

        memset(bits_next, 0, dst_len * sizeof(uint64_t));

        /* f^k = f^(k-1) * f      (mod 2) */
        xor_shift(bits_curr, src_len, bits_next, dst_len, 0);
        xor_shift(bits_curr, src_len, bits_next, dst_len, 1);
        xor_shift(bits_curr, src_len, bits_next, dst_len, 2);
        xor_shift(bits_curr, src_len, bits_next, dst_len, 3);
        xor_shift(bits_curr, src_len, bits_next, dst_len, 4);

        /* Count odd coefficients (number of set bits) */
        unsigned long long count = 0;
        for (size_t i = 0; i < dst_len; ++i)
            count += __builtin_popcountll(bits_next[i]);

        cache[k] = (seq_t)count;

        /* Prepare for next iteration */
        memcpy(bits_curr, bits_next, dst_len * sizeof(uint64_t));
    }

    computed_max = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
