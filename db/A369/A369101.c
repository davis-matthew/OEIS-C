/*
 * A369101  Dirichlet g.f.: zeta(s-3)^2 * (1 - 2^(4-s)) / zeta(s).
 * https://oeis.org/A369101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(1) = 1, a(2^e) = -(7*e-6)*8^(e-1)
 *            (e ≥ 1), a(p^e) = p^(3*e-3) * (1 + (e+1)*(p^3-1)) (p odd prime,
 *            e ≥ 1). The implementation factors n by trial division and
 *            multiplies the contributions per prime power
 * Range:     i8 n <= 4, i16 n <= 20, i32 n <= 575, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

static seq_t seq_pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined, but handle gracefully */
    if (n == 1) return 1;    /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;

    /* Handle factor 2 */
    if (m % 2 == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            e++;
        }
        seq_t coeff = (seq_t)7 * (seq_t)e - 6;      /* 7*e-6 */
        seq_t pow8  = seq_pow_seq((seq_t)8, e - 1); /* 8^(e-1) */
        seq_t contrib = -(coeff * pow8);
        result *= contrib;
    }

    /* Handle odd prime factors */
    for (idx_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            idx_t e = 0;
            while (m % d == 0) {
                m /= d;
                e++;
            }
            seq_t p = (seq_t)d;
            seq_t p_pow = seq_pow_seq(p, 3 * e - 3);   /* p^(3e-3) */
            seq_t p3_minus1 = seq_pow_seq(p, 3) - 1;   /* p^3 - 1 */
            seq_t inner = 1 + (seq_t)(e + 1) * p3_minus1;
            seq_t contrib = p_pow * inner;
            result *= contrib;
        }
    }

    /* If remaining m > 1, it's a prime > sqrt(original n) */
    if (m > 1) {
        seq_t p = (seq_t)m;       /* e = 1 for this prime */
        seq_t p_pow = seq_pow_seq(p, 0);       /* p^(3*1-3) = p^0 = 1 */
        seq_t p3_minus1 = seq_pow_seq(p, 3) - 1;
        seq_t inner = 1 + (seq_t)2 * p3_minus1; /* (e+1)=2 */
        seq_t contrib = p_pow * inner;
        result *= contrib;
    }

    return result;
}
