/*
 * A245489  a(n) = (1^n + (-2)^n + 4^n)/3.
 * https://oeis.org/A245489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1^n + (-2)^n + 4^n)/3 Recurrence: a(0)=1, a(1)=1, a(2)=7
 *            a(n+3) = 6*a(n+1) + 3*a(n+2) - 8*a(n) (for all n ≥ 0) This
 *            recurrence yields the same values as the closed form, and avoids
 *            overflow when the result still fits in seq_t
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;
    if (n == 2)          return (seq_t)7;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)1;   /* a(n-2)   */
    seq_t a2 = (seq_t)7;   /* a(n-1)   */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = a1 * (seq_t)6 + a2 * (seq_t)3 - a0 * (seq_t)8;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
