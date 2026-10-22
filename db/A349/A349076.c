/*
 * A349076  a(n) = U(n, 3*n), where U(n, x) is the Chebyshev polynomial of the
 *          second kind.
 * https://oeis.org/A349076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = U(n, 3*n) U(n,x) = Σ_{k=0}^{⌊n/2⌋} (-1)^k * binom(n-k, k)
 *            * (2x)^{n-2k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Summation result */
    seq_t sum = 0;

    /* Current binomial coefficient binom(n-k, k), starting with k=0 */
    seq_t binom = 1;

    /* Base for the power: (6 * n) */
    seq_t pow_base = (seq_t)6 * (seq_t)n;

    /* Compute (6n)^n for k = 0 */
    seq_t cur_pow = 1;
    for (idx_t i = 0; i < n; ++i)
        cur_pow *= pow_base;

    /* Square of the base, used for successive divisions of cur_pow */
    seq_t pow_base_sq = pow_base * pow_base;

    /* Sign of the current term: +1 for even k, -1 for odd k */
    bool sign = true;

    /* Upper bound for k */
    idx_t half = n / 2;

    for (idx_t k = 0; k <= half; ++k) {
        /* term = binom * (6n)^{n-2k} with appropriate sign */
        seq_t term = binom * cur_pow;
        if (sign)
            sum += term;
        else
            sum -= term;

        /* Prepare for next iteration (k+1), unless this is the last term */
        if (k == half)
            break;

        /* Update binomial coefficient:
           binom(n-(k+1), k+1) = binom(n-k, k) *
           (n-2k)*(n-2k-1) / ((n-k)*(k+1))   (exact division) */
        seq_t num1 = (seq_t)(n - 2 * k);
        seq_t num2 = (seq_t)(n - 2 * k - 1);
        seq_t denom1 = (seq_t)(n - k);
        seq_t denom2 = (seq_t)(k + 1);
        binom = binom * num1 * num2 / denom1 / denom2;

        /* Reduce power: divide by (6n)^2 to get (6n)^{n-2(k+1)} */
        cur_pow = cur_pow / pow_base_sq;

        /* Flip sign for next term */
        sign = !sign;
    }
    return sum;
}
