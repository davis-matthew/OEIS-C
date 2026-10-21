/*
 * A283971  a(n) = n except a(4*n + 2) = 2*n + 1.
 * https://oeis.org/A283971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n except when n ≡ 2 (mod 4), where a(n) = n/2 (i.e.
 *            2*(n/4)+1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* check whether n is of the form 4*k + 2 */
    if ((n & 3u) == 2u)
        return (seq_t)(n >> 1);   /* n/2 is 2*k + 1 */
    else
        return (seq_t)n;
}
