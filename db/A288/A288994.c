/*
 * A288994  a(n) = n*(n+3) when n is congruent to 0 or 3 (mod 4), and
 *          n*(n+3)/2 otherwise.
 * https://oeis.org/A288994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3) when n mod 4 == 0 or 3, otherwise a(n) =
 *            n*(n+3)/2
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + (seq_t)3);        /* n*(n+3) */
    idx_t r = n & 3;                          /* n mod 4 */
    if (r == 0 || r == 3)
        return prod;                          /* use full value */
    return prod / (seq_t)2;                  /* halve for other residues */
}
