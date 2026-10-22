/*
 * A373105  a(n) = sigma_10(n^2)/sigma_5(n^2).
 * https://oeis.org/A373105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 84, u128 n <= 1113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A373105 – OEIS A373105
    a(n) = product over prime powers p^e||n of (p^(10e+5)+1)/(p^5+1)
*/
#include <stdint.h>
#include <stdbool.h>

/* Local fast exponentiation: returns base^exp using only seq_t
   arithmetic.  exp is small (≤ 145 for n ≤ 10 000).  */
static seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1u) result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

/*  Term contributed by the prime power p^e in the multiplicative
    definition of a(n).  Formula: (p^(10e+5)+1)/(p^5+1).  */
static seq_t factor_term(uint64_t p, idx_t e) {
    seq_t p_pow5 = pow_seq_local((seq_t)p, 5u);      /* p^5          */
    seq_t denom   = p_pow5 + 1u;                     /* p^5 + 1      */
    idx_t exp     = 2u * e + 1u;                     /* exponent for p^5 */
    seq_t num     = pow_seq_local(p_pow5, exp);      /* (p^5)^(2e+1) = p^(10e+5) */
    num += 1u;                                      /* num = p^(10e+5)+1 */
    return num / denom;                             /* exact division */
}

/*  Public function as required by the build.  */
seq_t a(idx_t n) {
    if (n < 1u) return 0;          /* out of OEIS offset – value irrelevant */
    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2u; p * p <= m; ++p) {
        if (m % p == 0u) {
            idx_t e = 0u;
            while (m % p == 0u) {
                ++e;
                m /= p;
            }
            result *= factor_term(p, e);
        }
    }
    if (m > 1u) {
        /* remaining prime factor with exponent 1 */
        result *= factor_term(m, 1u);
    }
    return result;
}
