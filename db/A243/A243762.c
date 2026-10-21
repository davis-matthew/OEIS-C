/*
 * A243762  a(n) = 4*n^3 + 5.
 * https://oeis.org/A243762
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 5
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 4*n^3 + 5 in a signed 128‑bit type to avoid overflow
       for the values that fit in seq_t. idx_t is int64_t, so the
       conversion is safe. */
    __int128 sn = n;                 /* n as signed 128‑bit integer */
    __int128 val = 4 * sn * sn * sn + 5;
    return (seq_t)val;
}
