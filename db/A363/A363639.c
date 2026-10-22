/*
 * A363639  Expansion of Sum_{k>0} (1/(1 - k*x^k)^3 - 1).
 * https://oeis.org/A363639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^d * binomial(d+2,2) with offset 1
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence from OEIS A363639 (Expansion of Sum_{k>0} (1/(1 - k*x^k)^3 - 1))   */

#include <stddef.h>
#include <stdint.h>

/* fast exponentiation for seq_t with exponent idx_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is always >= 1 for the OEIS definition; return 0 for n==0 (unused) */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            seq_t base = (seq_t)(n / d);
            seq_t pow_term = pow_seq(base, d);
            seq_t binom_term = ((seq_t)(d + 2) * (seq_t)(d + 1)) / 2;
            sum += pow_term * binom_term;

            /* paired divisor e = n/d, skip if equal to d */
            idx_t e = n / d;
            if (e != d) {
                base = (seq_t)(n / e);          /* which equals d */
                pow_term = pow_seq(base, e);
                binom_term = ((seq_t)(e + 2) * (seq_t)(e + 1)) / 2;
                sum += pow_term * binom_term;
            }
        }
    }
    return sum;
}
