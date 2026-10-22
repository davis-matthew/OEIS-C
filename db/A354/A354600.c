/*
 * A354600  a(n) = Product_{k=0..9} floor((n+k)/10).
 * https://oeis.org/A354600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{9} ⌊(n+k)/10⌋
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k < 10; ++k)
    {
        /* Compute floor((n+k)/10) in a way that avoids overflow of n + k.
           The addition is performed in seq_t before the division. */
        seq_t term = ((seq_t)n + k) / 10;
        prod *= term;
    }
    return prod;
}
