/*
 * A326061  Sum of all other divisors of n except the largest proper divisor.
 *          a(1) = 0 by convention.
 * https://oeis.org/A326061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - largest proper divisor(n) = A000203(n) -
 *            A032742(n)
 * Range:     u8 n <= 119, u16 n <= 9723, u32 n <= 9723, u64 n <= 9723,
 *            u128 n <= 9723, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) is defined to be 0 */

    uint64_t tmp = n;              /* work on a 64‑bit copy of n */
    seq_t sigma = (seq_t)1;        /* product over prime factors   */
    seq_t n_seq = (seq_t)n;        /* n in seq_t arithmetic        */
    uint64_t spf_val = n;          /* smallest prime factor (LPD=n/p) */

    /* Trial division up to sqrt(n) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0) continue;

        if (spf_val == n) spf_val = p;   /* first non‑trivial factor */

        /* Sum of powers of p: 1 + p + p² + … + pᵉ  (e = multiplicity) */
        seq_t sum = (seq_t)1;
        seq_t pow  = (seq_t)1;
        while (tmp % p == 0) {
            tmp /= p;
            pow  = pow * (seq_t)p;
            sum += pow;
        }

        sigma = sigma * sum;
    }

    /* Remaining prime factor, if any */
    if (tmp > 1) {
        if (spf_val == n) spf_val = tmp;        /* n itself was prime */
        seq_t sum = (seq_t)tmp + (seq_t)1;      /* 1 + p  */
        sigma = sigma * sum;
    }

    /* Largest proper divisor is n / spf_val */
    seq_t lpd = n_seq / (seq_t)spf_val;
    return sigma - lpd;
}
