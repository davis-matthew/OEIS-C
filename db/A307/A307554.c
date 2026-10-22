/*
 * A307554  Longest path length in the n X n fiveleaper graph.
 * https://oeis.org/A307554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2 - 1 for n > 7; otherwise the known terms
 *            0,0,0,0,2,19,47
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Trivial table for n <= 7 */
    static const seq_t small[8] = {
        0,  /* a(1) */
        0,  /* a(2) */
        0,  /* a(3) */
        0,  /* a(4) */
        2,  /* a(5) */
        19, /* a(6) */
        47  /* a(7) */
    };

    if (n <= 7)
        return small[n - 1];

    /* For n >= 8 the sequence follows the conjecture a(n) = n^2 - 1 */
    seq_t nn = (seq_t)n;
    return nn * nn - 1;
}
