/*
 * A349497  a(n) is the smallest element in the continued fraction of the
 *          harmonic mean of the divisors of n.
 * https://oeis.org/A349497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest element in the continued fraction of the
 *            harmonic mean of the divisors of n. The harmonic mean equals n *
 *            τ(n) / σ(n), where τ is the divisor count and σ is the sum of
 *            divisors. The continued fraction of a rational number a/b is
 *            obtained by the Euclidean algorithm and its partial...
 * Range:     u8 n <= 35, u16 n <= 6147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9856 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* local helper: factor n (given as idx_t) and compute τ(n) and σ(n).
   The algorithm does simple trial division and works fast for the
   n that appear in the sequence (up to at least 10^4). */
static void tau_sigma(idx_t n, seq_t *tau_out, seq_t *sigma_out)
{
    idx_t temp = n;
    seq_t tau   = 1;
    seq_t sigma = 1;

    /* factor 2 separately */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            e++;
        }
        tau *= (seq_t)(e + 1);
        /* sum of powers 1 + 2 + 2^2 + ... + 2^e */
        seq_t sum = 1;
        seq_t pow = 1;
        for (idx_t i = 0; i < e; ++i) {
            pow *= 2;
            sum += pow;
        }
        sigma *= sum;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            tau *= (seq_t)(e + 1);
            /* sum of geometric series: 1 + p + p^2 + ... + p^e */
            seq_t sum  = 1;
            seq_t pow  = 1;
            for (idx_t i = 0; i < e; ++i) {
                pow *= (seq_t)p;
                sum += pow;
            }
            sigma *= sum;
        }
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        tau *= 2;
        sigma *= (seq_t)(1 + temp);
    }

    *tau_out   = tau;
    *sigma_out = sigma;
}

seq_t a(idx_t n)
{
    seq_t tau, sigma;
    tau_sigma(n, &tau, &sigma);

    seq_t num = (seq_t)n * tau;   /* n * τ(n)   */
    seq_t den = sigma;            /* σ(n)       */

    seq_t min_q = 0;
    bool first = true;

    /* Euclidean algorithm to compute continued fraction quotients
       and keep the smallest one. */
    while (den != 0) {
        seq_t q = num / den;
        if (first || q < min_q) {
            min_q = q;
            first = false;
        }
        seq_t rem = num % den;
        num = den;
        den = rem;
    }

    return min_q;
}
