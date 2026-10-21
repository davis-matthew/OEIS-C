/*
 * A247375  Numbers m such that floor(m/2) is a square.
 * https://oeis.org/A247375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { n^2 / 2 if n is even * { ((n-1)^2)/2 + 1 if n is odd
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0)          /* even index */
    {
        unsigned __int128 t = (unsigned __int128)n * (unsigned __int128)n;
        return (seq_t)(t / (unsigned __int128)2);
    }
    else                          /* odd index */
    {
        unsigned __int128 t = (unsigned __int128)(n - 1) * (unsigned __int128)(n - 1);
        return (seq_t)(t / (unsigned __int128)2 + 1);
    }
}
