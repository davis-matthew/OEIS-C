/*
 * A365341  a(n) = (5*n)!/(4*n+1)!.
 * https://oeis.org/A365341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)!/(4*n+1)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t start = 4*n + 2;
    idx_t end   = 5*n;

    /* Multiply all integers from 4*n+2 up to 5*n inclusive.
       For n = 0 or 1 the loop is skipped, giving result = 1. */
    for (idx_t k = start; k <= end; ++k)
        result *= (seq_t)k;

    return result;
}
