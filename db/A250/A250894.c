/*
 * A250894  Number of (n+1) X (4+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -265 - 21*2^n + 9604*3^(n-3) + (226-9*2^(3+n))*n +
 *            6*(7+9*2^n)*n^2 for n > 2 For n = 1, 2 the values are 1229 and
 *            3867 respectively
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer power for the opaque type seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Off‑by‑two base cases */
    if (n == 1)
        return (seq_t)1229;
    if (n == 2)
        return (seq_t)3867;
    /* n >= 3 */
    seq_t n_val = (seq_t)n;
    seq_t n_sq  = n_val * n_val;            /* n^2 */
    seq_t two_n = pow_seq((seq_t)2, n);     /* 2^n   */
    seq_t three_n_minus3 = pow_seq((seq_t)3, n - 3);  /* 3^(n-3) */

    /* Positive contributions */
    seq_t pos = 0;
    pos += (seq_t)9604 * three_n_minus3;
    pos += (seq_t)226 * n_val;
    pos += (seq_t)42 * n_sq;
    pos += (seq_t)54 * (two_n * n_sq);

    /* Negative contributions */
    seq_t neg = (seq_t)265;
    neg += (seq_t)72 * (two_n * n_val);
    neg += (seq_t)21 * two_n;

    /* Result: pos - neg */
    return pos - neg;
}
