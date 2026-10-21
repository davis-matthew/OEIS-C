/*
 * A243469  Denominators of the rational convergents to the periodic continued
 *          fraction 1/(2 + 1/(7 + 1/(2 + 1/(7 + ...)))).
 * https://oeis.org/A243469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 16*a(n-2) - a(n-4) for n >= 5; a(1) = 1, a(2) = 2, a(3) =
 *            15, a(4) = 32
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)15;
    if (n == 4) return (seq_t)32;

    /*  a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)  */
    seq_t a0 = (seq_t)1;  /* a(1)   */
    seq_t a1 = (seq_t)2;  /* a(2)   */
    seq_t a2 = (seq_t)15; /* a(3)   */
    seq_t a3 = (seq_t)32; /* a(4)   */

    seq_t an = a3;  /* placeholder for a(n) */

    /* Compute terms up to n using the 4th‑order linear recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)16 * a2 - a0;   /* a(i) = 16*a(i-2) - a(i-4) */

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
