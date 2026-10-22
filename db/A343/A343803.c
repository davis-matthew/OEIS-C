/*
 * A343803  a(n) = Sum_{k=1..n} k * (number of divisors of n <= k).
 * https://oeis.org/A343803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * (# of divisors of n that are ≤ k) *
 *            Equivalently: a(n) = τ(n) * n(n+1)/2 - (σ₂(n) - σ₁(n))/2 * where
 *            τ = number of divisors, σ₁ = sum of divisors, σ₂ = sum of
 *            squares of divisors
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*--------------------- Helper: fast exponentiation for seq_t ---------------------*/
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/*--------------------- Compute a(n) by factorisation ---------------------*/
static seq_t compute_a_value(idx_t n) {
    if (n == 1) return (seq_t)1;          /* special case: a(1) = 1 */

    idx_t        tau      = 1;            /* τ(n)  */
    seq_t        sigma1   = (seq_t)1;     /* σ₁(n) */
    seq_t        sigma2   = (seq_t)1;     /* σ₂(n) */

    idx_t       tmp = n;
    idx_t       p;
    for (p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            /* update τ */
            tau *= (e + 1);

            /* seq_t arithmetic with base p */
            seq_t p_seq = (seq_t)p;

            /* σ₁ term: (p^(e+1)-1)/(p-1) */
            seq_t pow_pe1      = pow_seq(p_seq, e + 1);
            seq_t sigma1_term  = (pow_pe1 - (seq_t)1) / (p_seq - (seq_t)1);
            sigma1 *= sigma1_term;

            /* σ₂ term: (p^(2(e+1))-1)/(p^2-1) */
            seq_t pow_p2e1     = pow_seq(p_seq, 2 * (e + 1));
            seq_t denom2       = p_seq * p_seq - (seq_t)1;
            seq_t sigma2_term  = (pow_p2e1 - (seq_t)1) / denom2;
            sigma2 *= sigma2_term;
        }
    }

    if (tmp > 1) {                          /* remaining prime factor */
        idx_t e = 1;
        tau *= (e + 1);

        seq_t p_seq = (seq_t)tmp;

        seq_t pow_pe1     = pow_seq(p_seq, e + 1); /* p^2 */
        seq_t sigma1_term = (pow_pe1 - (seq_t)1) / (p_seq - (seq_t)1);
        sigma1 *= sigma1_term;

        seq_t pow_p2e1    = pow_seq(p_seq, 2 * (e + 1)); /* p^4 */
        seq_t denom2      = p_seq * p_seq - (seq_t)1;
        seq_t sigma2_term = (pow_p2e1 - (seq_t)1) / denom2;
        sigma2 *= sigma2_term;
    }

    /* T = n*(n+1)/2 */
    seq_t T = ((seq_t)n) * (((seq_t)n) + (seq_t)1) / (seq_t)2;

    /* a(n) = τ * T - (σ₂ - σ₁)/2 */
    seq_t diff   = sigma2 - sigma1;
    seq_t half   = diff / (seq_t)2;
    seq_t result = ((seq_t)tau) * T - half;

    return result;
}

/*--------------------- Public function as required by the test harness ---------------------*/
seq_t a(idx_t n) {
    return compute_a_value(n);
}
