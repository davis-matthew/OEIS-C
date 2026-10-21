/*
 * A264850  a(n) = n*(n + 1)*(n + 2)*(7*n - 5)/12.
 * https://oeis.org/A264850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(7*n - 5)/12
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 292, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on 64‑bit builds */
    unsigned __int128 nn = n;
    unsigned __int128 prod = nn;
    prod *= (nn + 1);
    prod *= (nn + 2);
    prod *= (7 * nn - 5);
    prod /= 12;
    return (seq_t)prod;
}
