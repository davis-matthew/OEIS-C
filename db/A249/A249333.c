/*
 * A249333  Number of regions formed by extending the sides of a regular
 *          n-gon.
 * https://oeis.org/A249333
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+2)/2 for n even, * a(n) = (n^2+n+2)/2 for n odd *
 *            The sequence is defined for n >= 3
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    if ((n & 1ULL) == 0)  /* n even */
        return (ns * ns + (seq_t)2) / (seq_t)2;
    else                  /* n odd */
        return (ns * ns + ns + (seq_t)2) / (seq_t)2;
}
