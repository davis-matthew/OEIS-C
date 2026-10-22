/*
 * A358652  a(n) = n!*Sum_{m=1..floor((n+1)/2)} 1/(m*binomial(n-m,m-1)).
 * https://oeis.org/A358652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{m=1..floor((n+1)/2)} 1/(m*binomial(n-m,m-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factorial.h"
#include "../utils/gcd.h"

static inline seq_t mul_seq_t(seq_t x, seq_t y) { return x * y; }

seq_t a(idx_t n) {
    idx_t k = (n + 1) / 2;            /* floor((n+1)/2) */
    seq_t fac = factorial(n);         /* n! */
    seq_t sum = fac;                  /* term m = 1 contributes n! */
    seq_t term = fac;

    for (idx_t m = 1; m < k; ++m) {
        seq_t m_seq     = (seq_t)m;
        seq_t n_minus_m = (seq_t)(n - m);

        /* numerator = m^2 * (n - m) */
        seq_t numer = mul_seq_t(mul_seq_t(m_seq, m_seq), n_minus_m);

        /* denominator = (m+1)*(n-2m+1)*(n-2m) */
        seq_t denom = mul_seq_t(mul_seq_t((seq_t)(m + 1),
                                         (seq_t)(n - 2 * m + 1)),
                                 (seq_t)(n - 2 * m));

        /* cancel common factors to avoid overflow */
        seq_t g1 = gcd(numer, denom);
        numer /= g1;
        denom /= g1;

        seq_t g2 = gcd(term, denom);
        term /= g2;
        denom /= g2;                 /* denom should now be 1 */

        term = mul_seq_t(term, numer); /* exact division achieved */
        sum += term;
    }
    return sum;
}
