/*
 * A274544  Values of k such that 2*k-1 and 5*k-1 are both perfect squares.
 * https://oeis.org/A274544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274544: Values of k such that 2*k-1 and 5*k-1 are both perfect squares.
   Recurrence: a(n) = a(n-1) + 1442*a(n-2) - 1442*a(n-3) - a(n-4) + a(n-5)
   for n > 5 with initial terms
   a(1)=1, a(2)=13, a(3)=925, a(4)=18241, a(5)=1333345 */
seq_t a(idx_t n) {
    if (n == 0) { /* undefined for n<1; return 0 as placeholder */
        return 0;
    }
    /* Use base indices 1..5 */
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)13,
        (seq_t)925,
        (seq_t)18241,
        (seq_t)1333345
    };
    if (n <= 5) {
        return init[n - 1];
    }
    /* Sliding window for last five terms */
    seq_t a1 = init[0]; /* a(n-5) */
    seq_t a2 = init[1]; /* a(n-4) */
    seq_t a3 = init[2]; /* a(n-3) */
    seq_t a4 = init[3]; /* a(n-2) */
    seq_t a5 = init[4]; /* a(n-1) */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = a5
                       + (seq_t)1442 * a4
                       - (seq_t)1442 * a3
                       - a2
                       + a1;
        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = new_val;
    }
    return a5;
}
