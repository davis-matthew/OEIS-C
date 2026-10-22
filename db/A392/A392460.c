/*
 * A392460  Number of transitive relations on an n-set with exactly seven
 *          ordered pairs.
 * https://oeis.org/A392460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=3}^{14} coeff_k * C(n,k) with the coefficients
 *            listed below
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 46, u128 n <= 1042,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
    seq_t result = (seq_t)0;

    /* Coefficients for C(n,k), 3 ≤ k ≤ 14 */
    const struct { idx_t k; seq_t coeff; } terms[] = {
        {3, (seq_t)6},
        {4, (seq_t)644},
        {5, (seq_t)18550},
        {6, (seq_t)237210},
        {7, (seq_t)1748587},
        {8, (seq_t)8279224},
        {9, (seq_t)26645976},
        {10, (seq_t)59729040},
        {11, (seq_t)92862000},
        {12, (seq_t)96465600},
        {13, (seq_t)60540480},
        {14, (seq_t)17297280}
    };
    const size_t Terms = sizeof(terms) / sizeof(terms[0]);

    for (size_t i = 0; i < Terms; ++i) {
        idx_t k = terms[i].k;
        if (n < k) continue;
        seq_t comb = binom_small(n, k);
        result += terms[i].coeff * comb;
    }
    return result;
}
