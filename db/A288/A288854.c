/*
 * A288854  The unique longest sequence of squares where each number (after
 *          the first) is obtained by prefixing a single digit to its
 *          predecessor.
 * https://oeis.org/A288854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A288854: unique longest square chain; returns the n-th term.
   The chain is fixed: 25, 625, 5625, 75625, 275625.  For n > 5 the
   sequence is defined to be 0. */
seq_t a(idx_t n)
{
    static const seq_t chain[] = {
        25,
        625,
        5625,
        75625,
        275625
    };
    if (n == 0)
        return 0;
    if (n <= 5)
        return chain[n - 1];
    return 0;
}
