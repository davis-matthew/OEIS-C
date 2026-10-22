/*
 * A347651  Total domination number of the n-Sierpinski carpet graph.
 * https://oeis.org/A347651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 15*a(n-1) - 70*a(n-2) + 120*a(n-3) - 64*a(n-4) with a(1)
 *            = 4, a(2) = 24, a(3) = 172, a(4) = 1304
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)172;
    if (n == 4) return (seq_t)1304;

    /* Iteratively compute using the linear recurrence */
    seq_t a1 = (seq_t)4;      /* a(n-4)   */
    seq_t a2 = (seq_t)24;     /* a(n-3)   */
    seq_t a3 = (seq_t)172;    /* a(n-2)   */
    seq_t a4 = (seq_t)1304;   /* a(n-1)   */
    seq_t an;

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)15 * a4 - (seq_t)70 * a3 + (seq_t)120 * a2 - (seq_t)64 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
