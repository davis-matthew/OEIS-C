/*
 * A390310  a(n) = if(n mod 3 = 0, n/3, 4 * n + (-1)^(n mod 3)).
 * https://oeis.org/A390310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 3 == 0) ? n/3 : 4*n + ((n mod 3)==1 ? -1 : 1)
 * Range:     u8 n <= 64, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3u;
    if (r == 0u)
        return (seq_t)(n / 3u);
    else if (r == 1u)
        return (seq_t)4u * (seq_t)n - (seq_t)1u;
    else /* r == 2u */
        return (seq_t)4u * (seq_t)n + (seq_t)1u;
}
