/*
 * A294981  a(n) is the number of elements added at n-th stage to the
 *          structure of the cellular automaton of A294980.
 * https://oeis.org/A294981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, more
 * Formula:   a(n) = 1 for n = 1; a(n) = 3 for n = 2; a(n) = 6 if n ≡ 3,4,5
 *            (mod 6); a(n) = 18 otherwise (i.e. n ≡ 0,1,2 (mod 6) for n ≥ 3)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    /* for n >= 3, use modulo 6 */
    idx_t r = (n - 3) % 6;
    return (r < 3) ? (seq_t)6 : (seq_t)18;
}
