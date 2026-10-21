/*
 * A271476  Total number of burnt pancakes flipped using the Min-bar(n) greedy
 *          algorithm.
 * https://oeis.org/A271476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = even primitive recurrence derived from a(n) = 2^n n!
 *            ∑_{k=0}^{n-1} 1/(2^k k!) Recurrence: let t_1 = 2, t_n = 2n *
 *            (t_{n-1} + 1), then a(n) = t_n - n (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n ≥ 1 by definition of the sequence offset */
    seq_t t = (seq_t)2; /* t_1 */
    if (n == 1) return t - (seq_t)1; /* 2 - 1 = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tplus1 = t + (seq_t)1;                   /* t_{i-1} + 1 */
        seq_t factor = (seq_t)(i * (seq_t)2);          /* 2 * i */
        t = tplus1 * factor;                           /* t_i = 2i * (t_{i-1} + 1) */
    }
    return t - (seq_t)n;               /* a(n) = t_n - n */
}
