/*
 * A267613  Binary representation of the n-th iteration of the "Rule 185"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-1) - 100*a(n-2) for n > 3 Initial values: a(0) =
 *            1, a(1) = 1, a(2) = 1011, a(3) = 101111
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A267613 */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2)           return (seq_t)1011;
    if (n == 3)           return (seq_t)101111;

    /* Iterative recurrence */
    seq_t prev2 = (seq_t)1011;     /* a(2) */
    seq_t prev1 = (seq_t)101111;   /* a(3) */
    seq_t cur    = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 101*a(i-1) - 100*a(i-2) */
        cur = (seq_t)101 * prev1 - (seq_t)100 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
