/*
 * A323221  a(n) = n*(n + 5)*(n + 7)/6 + 1.
 * https://oeis.org/A323221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+5)*(n+7)/6 + 1
 * Range:     u8 n <= 7, u16 n <= 69, u32 n <= 2949, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 val = nn * (nn + 5u) * (nn + 7u);
    val /= 6u;
    return (seq_t)(val + 1u);
}
