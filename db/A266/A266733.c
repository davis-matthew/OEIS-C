/*
 * A266733  a(n) = 21*binomial(n+6,7).
 * https://oeis.org/A266733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)*(n+6)/240
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 prod = 1;
    for (idx_t i = 0; i < 7; ++i) {
        prod *= (unsigned __int128)(n + i);
    }
    prod /= 240;
    return (seq_t)prod;
}
