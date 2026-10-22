/*
 * A348407  a(n) = ((n+1)*3*2^(n+1) + 29*2^n + (-1)^n)/9.
 * https://oeis.org/A348407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1) * 3 * 2^(n + 1) + 29 * 2^n + (-1)^n) / 9
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* factor = 6n + 35 */
    seq_t factor = (seq_t)6 * (seq_t)n + 35;

    /* numerator = 2^n * (6n + 35) + (-1)^n */
    seq_t prod = pow2 * factor;
    if ((n & 1ULL) == 0)
        prod += 1;      /* (-1)^n = 1 for even n */
    else
        prod -= 1;      /* (-1)^n = -1 for odd n */

    /* division by 9 gives the sequence term */
    return prod / 9;
}
