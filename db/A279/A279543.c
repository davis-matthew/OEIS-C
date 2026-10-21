/*
 * A279543  a(n) = a(n-1) + 3^n * a(n-2) with a(0) = 1 and a(1) = 1.
 * https://oeis.org/A279543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 3^n * a(n-2),  a(0) = 1, a(1) = 1 */
seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Previous two terms: a(0) and a(1) */
    seq_t prev2 = (seq_t)1;   /* a(n-2), starts with a(0) */
    seq_t prev1 = (seq_t)1;   /* a(n-1), starts with a(1) */

    /* Keep track of 3^i; start with 3^1 = 3 */
    seq_t pow3 = (seq_t)3;

    /* Iterate from i = 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* pow3 becomes 3^i */
        pow3 *= (seq_t)3;

        /* Compute current term: a(i) = prev1 + 3^i * prev2 */
        seq_t curr = prev1 + pow3 * prev2;

        /* Shift window for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }

    /* After loop, prev1 holds a(n) */
    return prev1;
}
