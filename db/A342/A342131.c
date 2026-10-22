/*
 * A342131  a(n) = n/2 + floor(n/4) if n is even, otherwise (3*n+1)/2.
 * https://oeis.org/A342131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 + floor(n/4) if n even, a(n) = (3*n+1)/2 if n odd
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;

    /* Even n: a(n) = n/2 + floor(n/4) */
    if ((n & 1ULL) == 0)
    {
        return (nn / 2) + (nn / 4);
    }

    /* Odd n: a(n) = (3*n + 1) / 2 */
    return ((seq_t)3 * nn + (seq_t)1) / (seq_t)2;
}
