/*
 * A281481  a(n) = 2^(n - 1) * (2^n + 1) + 1.
 * https://oeis.org/A281481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * (2^n + 1) + 1 (a(0) = 2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    /* compute 2^n using repeated left shifts, safe for all seq_t */
    seq_t pow2_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n <<= 1;            /* pow2_n = 2^n */

    seq_t pow2_prev = pow2_n >> 1;  /* 2^(n-1) */

    return pow2_prev * (pow2_n + (seq_t)1) + (seq_t)1;
}
