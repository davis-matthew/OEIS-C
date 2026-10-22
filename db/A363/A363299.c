/*
 * A363299  a(n) is the sum of the n-th powers of the terms of row 4 of
 *          Pascal's triangle.
 * https://oeis.org/A363299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2*4^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* fast exponentiation on seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow4n = pow_seq((seq_t)4, n);   /* 4^n */
    seq_t pow6n = pow_seq((seq_t)6, n);   /* 6^n */
    return (seq_t)2 + (seq_t)2 * pow4n + pow6n;
}
