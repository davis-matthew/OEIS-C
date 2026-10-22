/*
 * A300622  Denominators of sequence whose exponential self-convolution yields
 *          sequence 1, 2, 3, 5, 7, 11, 13, ... (1 with primes).
 * https://oeis.org/A300622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^( (n>>1) - ((n&3)==3?1:0) )
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t exp = n >> 1;                 /* floor(n/2) */
    if ((n & 3) == 3)                   /* subtract 1 when n ≡ 3 (mod 4) */
        --exp;
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)     /* compute 2^exp */
        res <<= 1;
    return res;
}
