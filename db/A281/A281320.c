/*
 * A281320  Number of n X 2 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 2, a(3) = 28, a(4) = 304 For n ≥ 5, a(n) =
 *            16·a(n‑1) – 72·a(n‑2) + 64·a(n‑3) – 16·a(n‑4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A281320.  The algorithm is
   linear in n, uses only 4 sequence values at a time, and
   works for all accepted integer types of seq_t. */
seq_t a(idx_t n)
{
    /* Basic cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)28;
    if (n == 4) return (seq_t)304;

    /* a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)28;  /* a(3) */
    seq_t a4 = (seq_t)304; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence */
        seq_t next = (seq_t)16 * a4
                   - (seq_t)72 * a3
                   + (seq_t)64 * a2
                   - (seq_t)16 * a1;

        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
