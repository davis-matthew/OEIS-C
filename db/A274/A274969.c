/*
 * A274969  Number of walks in the first quadrant starting and ending at (0,0)
 *          consisting of 3n steps taken from {E=(1, 0), D=(-1, 1), S=(0,
 *          -1)}, no S step occurring before the final E step.
 * https://oeis.org/A274969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3n, n) - 2*binomial(3n, n-1) + binomial(3n, n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (see OEIS sequence A274969) */

#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) for arbitrary sized seq_t.
   The routine multiplies and divides by the GCD at each step to keep
   intermediate values as small as possible. */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;   /* running numerator factor */
        seq_t den = i;           /* running denominator factor */

        /* cancel common factors between result and denominator */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* cancel common factors between numerator and denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply the remaining numerator factor */
        result *= num;

        /* after cancellation den must be 1 */
    }
    return result;
}

seq_t a(idx_t n)
{
    /* offset 0: a(0)=1, a(1)=1 */
    if (n <= 1) return 1;

    idx_t m = 3 * n;

    seq_t term1 = _binom(m, n);
    seq_t term2 = _binom(m, n - 1);
    seq_t term3 = _binom(m, n - 2);

    /* Compute term1 - 2*term2 + term3 without risking unsigned underflow. */
    seq_t two_term2 = term2 + term2;

    seq_t result;
    if (term1 >= two_term2)
    {
        /* safe: term1 - two_term2 is non‑negative */
        result = term1 - two_term2;
        result += term3;
    }
    else
    {
        /* term1 < two_term2: compute as term3 - (two_term2 - term1) */
        seq_t diff = two_term2 - term1;
        result = term3 - diff;
    }
    return result;
}
