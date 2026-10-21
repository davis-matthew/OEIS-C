/*
 * A294573  a(n) = n! * [x^n] exp((n+1)*x)*BesselI(1,2*x)/x.
 * https://oeis.org/A294573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/2⌋} binomial(n,2k) * Catalan(k) *
 *            (n+1)^{n-2k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the n-th term of OEIS sequence A294573.
 * It works for any seq_t and idx_t types provided by the build,
 * without using floating point or external libraries.
 */

#include <stdint.h>   /* only for definition of standard integer types */

/* Helper: binary exponentiation for seq_t base and idx_t exponent. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return 1;

    idx_t max_k = n / 2;                 /* floor(n/2)   */
    seq_t sum    = 0;

    /* binomial(n, 0) = 1 */
    seq_t binom  = 1;
    /* Catalan(0) = 1 */
    seq_t cat    = 1;
    /* pow = (n+1)^n */
    seq_t pow    = pow_seq((seq_t)(n + 1), n);

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* term = binom * cat * pow */
        sum += binom * cat * pow;

        if (k == max_k) break;          /* last term processed */

        /* Update binomial: C(n,2(k+1)) from C(n,2k) */
        seq_t num1 = (seq_t)(n - 2 * k);
        seq_t num2 = (seq_t)(n - 2 * k - 1);
        seq_t den1 = (seq_t)(2 * k + 2);
        seq_t den2 = (seq_t)(2 * k + 1);
        binom = binom * num1 * num2 / (den1 * den2);

        /* Update Catalan: Cat(k+1) from Cat(k) */
        cat = cat * (seq_t)2 * (seq_t)(2 * k + 1) / (seq_t)(k + 2);

        /* Update power: (n+1)^{n-2(k+1)} from (n+1)^{n-2k} */
        seq_t factor = (seq_t)(n + 1) * (seq_t)(n + 1);
        pow = pow / factor;
    }
    return sum;
}
