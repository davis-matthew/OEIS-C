/*
 * A382037  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * B(x)^3)
 *          ), where B(x) = 1 + x*B(x)^3 is the g.f. of A001764.
 * https://oeis.org/A382037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Sum_{k=0..n-1} binomial(3*n,k) * * (n+1)^{n-k-1} * *
 *            (n-1)!/(n-k-1)! , n > 0 * a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A382037_H_
#define _A382037_H_

#include <stdint.h>

/* Fast exponentiation for seq_t base and non‑negative int exponent.
 * Only integer arithmetic is used, compatible with C11 and C++17. */
static inline seq_t pow_seq(seq_t base, int exp) {
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* External linkage as required. */
seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;

    /* binomial(3*n,0) = 1 */
    seq_t bin = 1;

    /* (n-1)!/(n-1)! = 1 for k = 0 */
    seq_t ratio = 1;

    /* (n+1)^(n-1) for k = 0 */
    seq_t power = pow_seq((seq_t)(n + 1), (int)(n - 1));

    for (idx_t k = 0; k < n; ++k) {
        /* term = bin * power * ratio   (the integer derived term) */
        seq_t term = bin * power;
        term = term * ratio;
        sum += term;

        /* prepare for next k, unless we are done */
        if (k + 1 < n) {
            /* binomial recurrence: bin_{k+1} = bin_k * (3*n - k)/(k+1) */
            bin = bin * (seq_t)(3 * n - k);
            bin = bin / (seq_t)(k + 1);

            /* ratio recurrence: (n-1)!/(n-k-2)! = ratio_k * (n-1-k) */
            ratio = ratio * (seq_t)(n - 1 - k);

            /* power recurrence: (n+1)^{n-k-2} = power_k / (n+1) */
            power = power / (seq_t)(n + 1);
        }
    }

    return sum;
}

#endif /* _A382037_H_ */
