/*
 * A370058  a(n) = 4*(4*n+3)!/(3*n+4)!.
 * https://oeis.org/A370058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(4*n+3)!/(3*n+4)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 1;
    idx_t lo = 3*n + 5;
    idx_t hi = 4*n + 3;

    for (idx_t k = lo; k <= hi; ++k)
        result *= (seq_t)k;

    return ((seq_t)4) * result;
}
