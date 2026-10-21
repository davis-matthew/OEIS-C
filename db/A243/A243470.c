/*
 * A243470  Numerators of the rational convergents to the periodic continued
 *          fraction 1/(2 + 1/(7 + 1/(2 + 1/(7 + ...)))).
 * https://oeis.org/A243470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243470
   a(1) = 1, a(2) = 7, a(3) = 15, a(4) = 112
   recurrence for n ≥ 5: a(n) = 16*a(n-2) – a(n-4)   (OEIS 4th‑order recurrence) */

seq_t a(idx_t n)
{
    /* Fast paths for the first four terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)112;

    /* Keep the last four computed values:
       a(n-4), a(n-3), a(n-2), a(n-1)  */
    seq_t a_n_4 = (seq_t)1;   /* a(1)   */
    seq_t a_n_3 = (seq_t)7;   /* a(2)   */
    seq_t a_n_2 = (seq_t)15;  /* a(3)   */
    seq_t a_n_1 = (seq_t)112; /* a(4)   */
    seq_t a_n;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 16*a(i-2) – a(i-4) */
        a_n = (seq_t)16 * a_n_2 - a_n_4;

        /* shift the window for the next iteration */
        a_n_4 = a_n_3;
        a_n_3 = a_n_2;
        a_n_2 = a_n_1;
        a_n_1 = a_n;
    }

    /* After the loop, a_n_1 holds a(n) */
    return a_n_1;
}
