/*
 * A392400  a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-k,3*k).
 * https://oeis.org/A392400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-k,3*k) Recurrence:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=11
 *            a(n)=3*a(n-1)-a(n-2)+7*a(n-3)-a(n-4) for n≥4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 11;

    /* Last four computed values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = 1; /* a(0) */
    seq_t a1 = 1; /* a(1) */
    seq_t a2 = 2; /* a(2) */
    seq_t a3 = 11;/* a(3) */
    seq_t res = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 3*a3 - a2 + 7*a1 - a0 */
        res = (seq_t)3 * a3 - a2 + (seq_t)7 * a1 - a0;
        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return res;
}
