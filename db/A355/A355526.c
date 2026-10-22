/*
 * A355526  Maximal difference between adjacent prime indices of n, or k if n
 *          is the k-th prime.
 * https://oeis.org/A355526
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   maximal difference between adjacent prime indices of n, or k if
 *            n is the k‑th prime
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* Provides factor_u64 */
#include "../utils/compute_pi.h"   /* Provides compute_pi */

/* Comparator for qsort on idx_t values */
static int cmp_idx(const void *a, const void *b) {
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Compute A355526(n) for n >= 2 */
seq_t a(idx_t n) {
    if (n < 2) return 0;   /* behaviour below offset is irrelevant */

    /* Factor n into primes and exponents (max 64 distinct primes for 64‑bit numbers) */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    /* Build the multiset of prime indices (with multiplicity) */
    idx_t indices[128];  /* ample space for repetitions (max exponent <= 64) */
    size_t off = 0;

    for (size_t i = 0; i < cnt; ++i) {
        /* index of the prime factor */
        seq_t pi_val = compute_pi(pr[i]);           /* pi(p), exact value in seq_t */
        idx_t idx = (idx_t)pi_val;                  /* cast is safe: pi(p) <= p <= 2^64-1 */

        /* repeat according to multiplicity */
        for (uint64_t k = 0; k < ex[i]; ++k) {
            indices[off++] = idx;
        }
    }

    /* Special case: single prime factor (n is a prime or a prime power) */
    if (off == 1) {
        return (seq_t)indices[0];
    }

    /* Sort indices and find maximal difference between adjacent values */
    qsort(indices, off, sizeof(idx_t), cmp_idx);

    seq_t max_diff = 0;
    for (size_t i = 1; i < off; ++i) {
        idx_t diff = indices[i] - indices[i - 1];
        if (diff > max_diff) {
            max_diff = diff;
        }
    }
    return max_diff;
}
