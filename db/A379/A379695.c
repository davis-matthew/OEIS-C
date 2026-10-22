/*
 * A379695  Number of equivalence classes of regular vines in dimension n.
 * https://oeis.org/A379695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n ≤ 3 a(n) = Σ_{k=0}^{m} coeff_k * 2^{
 *            ((n-2)*(n-3)/2 - 1) - k*(n-k-2) } where m = floor(n/2) - 1,
 *            coeff_k = 2 when k == m, else 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small indices */
    if (n <= 3) return (seq_t)1;

    /* Base exponent: ((n-2)*(n-3)/2) - 1 */
    idx_t n_minus_2 = n - 2;
    idx_t n_minus_3 = n - 3;
    int64_t baseExp = (int64_t)(n_minus_2 * n_minus_3 / 2) - 1;

    /* m = floor(n/2) - 1 */
    idx_t m = n / 2 - 1;

    /* Find minimal exponent to avoid negative shifts */
    int64_t minExp = baseExp;
    for (idx_t k = 0; k <= m; ++k) {
        int64_t exp = baseExp - (int64_t)k * ((int64_t)n - (int64_t)k - 2);
        if (exp < minExp) minExp = exp;
    }
    int64_t shiftT = (minExp < 0) ? -minExp : 0;   /* amount to shift right later */

    /* Sum terms, all exponents shifted by shiftT so they are non‑negative */
    seq_t sum = 0;
    for (idx_t k = 0; k <= m; ++k) {
        seq_t coeff = (k == m) ? (seq_t)2 : (seq_t)1;
        int64_t expAdj = baseExp - (int64_t)k * ((int64_t)n - (int64_t)k - 2) + shiftT;
        seq_t term = coeff << (idx_t)expAdj;
        sum += term;
    }

    /* Remove the common power of two that we added */
    sum >>= shiftT;
    return sum;
}
