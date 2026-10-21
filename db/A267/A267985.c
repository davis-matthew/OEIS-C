/*
 * A267985  Numbers congruent to {7, 13} mod 30.
 * https://oeis.org/A267985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n-17 for even n; 15*n-8 for odd n
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset not used, safe fallback */
    seq_t prod = (seq_t)15 * n;          /* 15*n, cast to seq_t explicitly */
    if ((n & 1) == 0)                    /* even n */
        return prod - (seq_t)17;
    else                                 /* odd n */
        return prod - (seq_t)8;
}
