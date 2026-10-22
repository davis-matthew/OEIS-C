/*
 * A379992  Expansion of e.g.f. exp(-3*x)/(exp(-x) - x)^2.
 * https://oeis.org/A379992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)*(k-1)^(n-k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer exponentiation: base^exp, exp is unsigned */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t P = 1; /* P = n!/(n-k)! for current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update P_k = P_{k-1} * (n - (k-1)) */
        seq_t mul = (seq_t)(n - k + 1);
        P *= mul;

        seq_t term = P;
        term *= (seq_t)(k + 1);
        seq_t powv = pow_uint((seq_t)(k - 1), n - k);
        term *= powv;

        sum += term;
    }

    /* Add the k = 0 term: (-1)^n */
    if ((n & 1) == 0) {
        sum += (seq_t)1;
    } else {
        sum -= (seq_t)1;
    }

    return sum;
}
