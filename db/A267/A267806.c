/*
 * A267806  a(0) = a(1) = 1; for n>1, a(n) = (a(n-1) mod 2) + a(n-2).
 * https://oeis.org/A267806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/3) + (1 + (-1)^n)/2 (equivalently, a(n) =
 *            floor((n+2)/3) + (n even ? 1 : 0) )
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor((n+2)/3) using the index type to avoid overflow on very large n */
    idx_t tmp = (n + 2) / 3;
    seq_t result = (seq_t)tmp;

    /* add 1 for even n, 0 for odd n */
    if ((n & (idx_t)1) == 0)
        result += (seq_t)1;

    return result;
}
