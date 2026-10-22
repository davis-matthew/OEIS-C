/*
 * A368923  Number of congruences of the 0-twisted Brauer monoid of degree n.
 * https://oeis.org/A368923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n + 1)/2 if n is odd * a(n) = (3*n + 14)/2 if n is
 *            even and n >= 4 * a(0) = 2, a(1) = 2, a(2) = 5, a(3) = 5
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small base cases first. */
    if (n == 0u)
        return (seq_t)2;
    if (n == 1u)
        return (seq_t)2;
    if (n == 2u)
        return (seq_t)5;
    if (n == 3u)
        return (seq_t)5;

    /* For n >= 4 use the closed‑form formulas. */
    if ((n & 1u) != 0u)          /* odd n */
        return ((seq_t)3 * (seq_t)n + (seq_t)1) / (seq_t)2;
    else                          /* even n */
        return ((seq_t)3 * (seq_t)n + (seq_t)14) / (seq_t)2;
}
