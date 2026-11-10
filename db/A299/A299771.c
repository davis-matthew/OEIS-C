/*
 * A299771  a(n) is the number of elements added at n-th stage in the
 *          structure of the finite cellular automaton of A299770.
 * https://oeis.org/A299771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini, full
 * Formula:   a(n) = A299770(n) - A299770(n-1) for 1 <= n <= 10, with
 *            A299770(0) = 0; a(n) = 0 for n > 10 (the sequence is finite)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A299770_a(idx_t n)
{
    static const seq_t seq[10] = {
        1, 5, 13, 21, 33,
        49, 65, 73, 97, 105
    };
    if (n <= 10)
        return seq[n - 1u];
    return seq[9];
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 10)
        return (seq_t)0;
    if (n == 1)
        return A299770_a(1);
    return A299770_a(n) - A299770_a(n - 1);
}
