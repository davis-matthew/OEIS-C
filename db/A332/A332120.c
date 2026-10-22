/*
 * A332120  a(n) = 2*(10^(2n+1)-1)/9 - 2*10^n.
 * https://oeis.org/A332120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n repeated '2's) followed by '0' followed by (n repeated
 *            '2's) ; equivalently a(n) = 2*(10^(2n+1)-1)/9 - 2*10^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t v = 0;
    for (idx_t i = 0; i < n; ++i)
        v = v * (seq_t)10 + (seq_t)2;      /* build the first block of 2's */

    v *= (seq_t)10;                       /* insert the central 0 (no addition needed) */

    for (idx_t i = 0; i < n; ++i)
        v = v * (seq_t)10 + (seq_t)2;      /* finalize with the second block of 2's */

    return v;
}
