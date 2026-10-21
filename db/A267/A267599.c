/*
 * A267599  Binary representation of the n-th iteration of the "Rule 177"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 1001 a(n) = 101*a(n-1) - 100*a(n-2)
 *            for n >= 3 (Binary representation of the n‑th iteration of Rule
 *            177 starting from a single ON cell)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1001;

    /* Initialize for the recurrence */
    seq_t prev2 = (seq_t)1;      /* a(n-2)  = a(1) */
    seq_t prev1 = (seq_t)1001;   /* a(n-1)  = a(2) */
    seq_t curr  = 0;             /* will hold a(n) */

    /* Compute a(n) iteratively */
    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)101 * prev1 - (seq_t)100 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
