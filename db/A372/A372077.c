/*
 * A372077  The sequence T_{3,2}(n,3).
 * https://oeis.org/A372077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   x^2/(1-3*x+3*x^2-4*x^3):
   a(0) = 0, a(1) = 0, a(2) = 1
   a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3)   for n ≥ 3
   (equivalently a(n) = 3*a(n-1) + 4*a(n-3) - 3*a(n-2) to avoid unsigned
   underflow). */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = 0;   /* a(0)   */
    seq_t a1 = 0;   /* a(1)   */
    seq_t a2 = 1;   /* a(2)   */
    seq_t an = 0;   /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t temp = 3 * a2 + 4 * a0;   /* 3*a(n-1) + 4*a(n-3) */
        an = temp - 3 * a1;             /* subtract 3*a(n-2) */
        /* shift window forward */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
