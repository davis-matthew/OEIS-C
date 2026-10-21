/*
 * A290196  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 721", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 * (10^n - 1) / 9 for n >= 4; a(0) = 1, a(1) = 10, a(2)
 *            = 101, a(3) = 1101
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)1101;

    seq_t rep = 1;                // repunit of length 1
    for (idx_t i = 2; i <= n; ++i) {
        rep = rep * (seq_t)10 + (seq_t)1;   // append a digit 1
    }
    return rep * (seq_t)10;                     // trail with 0
}
