/*
 * A366839  Sum of even prime factors of 2n, counted with multiplicity.
 * https://oeis.org/A366839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(v2(n)+1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return twice the exponent of 2 in n plus 2. */
seq_t a(idx_t n)
{
    idx_t c = 0;
    /* count trailing zeros (i.e., the exponent of 2). */
    while ((n & 1) == 0)
    {
        c++;
        n >>= 1;
    }
    return 2 * ((seq_t)c + 1);
}
