/*
 * A341671  Solutions y of the Diophantine equation 3*(x^2+x+1) = y^2.
 * https://oeis.org/A341671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) for n ≥ 4 Initial values:
 *            a(1)=3, a(2)=39, a(3)=543 (OEIS sequence A341671) The function
 *            recomputes the recurrence from scratch each time, so no dynamic
 *            allocation or memoisation is required
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n ≤ 0 */

    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)39;
    if (n == 3) return (seq_t)543;

    seq_t a1 = (seq_t)3;      /* a(n-3) */
    seq_t a2 = (seq_t)39;     /* a(n-2) */
    seq_t a3 = (seq_t)543;    /* a(n-1) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)15 * a3 - (seq_t)15 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
