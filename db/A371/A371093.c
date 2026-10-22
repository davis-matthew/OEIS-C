/*
 * A371093  a(n) is the 2-adic valuation of 3n+1.
 * https://oeis.org/A371093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the exponent of 2 in (3*n + 1), i.e. v₂(3*n+1)
 * Range:     u8 n <= 84, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * ((seq_t)3) + ((seq_t)1);
    seq_t v = 0;
    while ((x % ((seq_t)2)) == (seq_t)0)
    {
        v++;
        x /= ((seq_t)2);
    }
    return v;
}
