/*
 * A377111  a(n) = coefficient of sqrt(3) in the expansion of (2 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of sqrt(3) in (2 + sqrt(2) + sqrt(3))^n
 *            Recurrence: a(n) = 8*a(n-1) - 14*a(n-2) - 8*a(n-3) + 23*a(n-4),
 *            with a(0)=0, a(1)=1, a(2)=4, a(3)=21
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)21;

    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)4;   /* a(n-2) */
    seq_t a3 = (seq_t)21;  /* a(n-1) */
    seq_t ai = 0;          /* current a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)8 * a3 - (seq_t)14 * a2 - (seq_t)8 * a1 + (seq_t)23 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return ai;
}
