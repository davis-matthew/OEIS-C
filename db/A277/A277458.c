/*
 * A277458  Expansion of e.g.f. -1/(1-LambertW(-x)).
 * https://oeis.org/A277458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   * a(0) = -1 * a(n) = n! * Σ_{k=1..n} (-1)^{k+1} * k *
 *            n^{\,n-k-1} / (n-k)! for n >= 1 * * This implementation
 *            evaluates the sum directly. * It only uses integer arithmetic
 *            and works for any seq_t/idx_t pair. * All intermediate values
 *            are computed with the native integer type seq_t, * which may
 *            be...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == (idx_t)0) return (seq_t)(-1);
    if (n == (idx_t)1) return (seq_t)1;

    /* n^{n-2}  (used for k = 1) */
    seq_t pow = pow_seq((seq_t)n, n - (idx_t)2);

    seq_t result = (seq_t)0;
    seq_t falling = (seq_t)1;   /* (n)_0 */

    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* update falling factorial: (n)_k */
        falling *= (seq_t)(n - (k - (idx_t)1));

        /* sign: +1 for odd k, -1 for even k */
        seq_t sign = (k & (idx_t)1) ? (seq_t)1 : (seq_t)-1;

        if (k < n)
        {
            /* term for k < n: k * falling * n^{n-k-1} */
            seq_t term = sign * (seq_t)k * falling * pow;
            result += term;
            /* prepare next power: divide by n */
            pow /= (seq_t)n;
        }
        else   /* k == n : term reduces to sign * falling ( = sign * n! ) */
        {
            result += sign * falling;
        }
    }

    return result;
}
