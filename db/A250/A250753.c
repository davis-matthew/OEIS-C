/*
 * A250753  Number of (n+1) X (6+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (357*3^n - 168*2^n + 69)/2
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, unused case */

    /* Compute 3^n and 2^n with arbitrary‑precision integer type seq_t */
    seq_t pow3 = seq_pow((seq_t)3, n);
    seq_t pow2 = seq_pow((seq_t)2, n);

    /* Apply the closed form */
    seq_t term = pow3 * (seq_t)357;      /* 357*3^n  */
    term -= pow2 * (seq_t)168;           /* -168*2^n */
    term += (seq_t)69;                   /* +69       */
    term /= (seq_t)2;                    /* divide by 2 */

    return term;
}
