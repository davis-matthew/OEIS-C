/*
 * A350362  2-tone chromatic number of an n-cycle.
 * https://oeis.org/A350362
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 for n = 3 or 4 or 7; otherwise 5 for n ≥ 5. (n < 3 not
 *            defined)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3 || n == 4 || n == 7)
        return (seq_t)6;
    if (n >= 5)
        return (seq_t)5;
    return (seq_t)0;   /* n < 3: offset not defined, return 0 */
}
