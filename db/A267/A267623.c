/*
 * A267623  Binary representation of the middle column of the "Rule 187"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 10, a(n) = 10*a(n-1) + 1 (n ≥ 2) (Derived from
 *            the recurrence a(n) = 11*a(n-1) – 10*a(n-2).)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 10;

    seq_t val = 10;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = 10 * val + 1;
    }
    return val;
}
