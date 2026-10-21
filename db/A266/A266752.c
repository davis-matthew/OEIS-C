/*
 * A266752  Binary representation of the n-th iteration of the "Rule 163"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-1) + 10 (for n ≥ 2), with a(0) = 1, a(1) = 100.
 *            That recurrence generates the binary‑pattern values in the OEIS
 *            sequence A266752
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)100;

    seq_t val = (seq_t)100;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)   /* iterate up to n using the recurrence */
        val = (seq_t)100 * val + (seq_t)10;

    return val;
}
