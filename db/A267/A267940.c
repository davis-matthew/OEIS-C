/*
 * A267940  Binary representation of the n-th iteration of the "Rule 253"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2) for n>3; a(0)=1, a(1)=11,
 *            a(2)=11111, a(3)=1111111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 1;
    if (n == 1u) return 11;
    if (n == 2u) return 11111;
    if (n == 3u) return 1111111;

    seq_t p2 = 11111;          /* a(n-2) */
    seq_t p1 = 1111111;        /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 101 * p1 - 100 * p2;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
