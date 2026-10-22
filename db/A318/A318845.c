/*
 * A318845  a(n) = Sum_{d|n} (-1)^(n/d+1) * Sum_{j|d} sigma(j), where sigma(j)
 *          = sum of divisors of j (A000203).
 * https://oeis.org/A318845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * Sum_{j|d} sigma(j)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 765, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Multiplicative formula (OEIS A318845) is used:
 *    a(2^e) = (e+1)(e+2)/2
 *    a(p^e) (p odd prime) =
 *      [ 2p^(e+3) – (e^2+5e+6)p^2 + (2e^2+8e+6)p – (e^2+3e+2) ]
 *      -----------------------------------------------
 *                     2(p‑1)^3
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* small exponentiation: p^exp, exp is small (≤ 64). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e_u = exps[i];
        idx_t e = (idx_t)e_u;
        seq_t a_pe;

        if (p == 2) {                      /* a(2^e) */
            seq_t ep = (seq_t)e;
            a_pe = ((ep + (seq_t)1) * (ep + (seq_t)2)) / (seq_t)2;
        } else {                           /* a(p^e), p odd prime */
            seq_t ep = (seq_t)e;
            seq_t p_seq = (seq_t)p;

            seq_t p_pow_e_plus3 = pow_seq(p_seq, e + 3);
            seq_t p_sq = p_seq * p_seq;
            seq_t e_sq = ep * ep;

            seq_t term1 = (seq_t)2 * p_pow_e_plus3;
            seq_t term2 = (e_sq + (seq_t)5 * ep + (seq_t)6) * p_sq;
            seq_t term3 = (seq_t)2 * e_sq + (seq_t)8 * ep + (seq_t)6;
            term3 = term3 * p_seq;
            seq_t term4 = e_sq + (seq_t)3 * ep + (seq_t)2;

            seq_t positive = term1 + term3;
            seq_t negative = term2 + term4;
            seq_t numerator = positive - negative;

            seq_t denominator = (seq_t)2 *
                                (p_seq - (seq_t)1) *
                                (p_seq - (seq_t)1) *
                                (p_seq - (seq_t)1);

            a_pe = numerator / denominator;
        }

        result *= a_pe;
    }

    return result;
}
