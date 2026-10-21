/*
 * A267595  Binary representation of the n-th iteration of the "Rule 173"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 101*a(n-1)-100*a(n-2) for n>3, with initial values
 *            a(0)=1, a(1)=10, a(2)=1111, a(3)=111111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 1111;
    if (n == 3) return 111111;

    seq_t prev2 = 1111;      /* a(n-2) */
    seq_t prev1 = 111111;    /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 101 * prev1 - 100 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
