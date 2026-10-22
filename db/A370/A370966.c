/*
 * A370966  a(n) = number of max-closed 2 X 2 X n relations.
 * https://oeis.org/A370966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35/6*6^n - 6*4^n + 2/3*3^n + 1/2*2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0u)
        return (seq_t)1;

    /* Compute powers needed for the formula.
       6^(n-1), 3^(n-1), 2^(n-1) are calculated by repeated multiplication
       up to n-1. Then 4^n is calculated separately by a loop up to n.   */
    seq_t pow2_shift = (seq_t)1;   /* 2^(n-1) */
    seq_t pow3_shift = (seq_t)1;   /* 3^(n-1) */
    seq_t pow6_shift = (seq_t)1;   /* 6^(n-1) */

    for (idx_t i = 1; i < n; ++i) {
        pow2_shift *= (seq_t)2;
        pow3_shift *= (seq_t)3;
        pow6_shift *= (seq_t)6;
    }

    seq_t pow4 = (seq_t)1;         /* 4^n */
    for (idx_t i = 1; i <= n; ++i) {
        pow4 *= (seq_t)4;
    }

    /* Assemble the result using only integer operations.          */
    seq_t result = (seq_t)0;
    result += (seq_t)35 * pow6_shift;   /* 35*6^(n-1) */
    result -= (seq_t)6 * pow4;          /* -6*4^n */
    result += (seq_t)2 * pow3_shift;    /* +2*3^(n-1) */
    result += pow2_shift;               /* +2^(n-1) */

    return result;
}
