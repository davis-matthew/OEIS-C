/*
 * A371596  The number of unilevel points (unique points at their height) on
 *          Delannoy paths ending when x = n.
 * https://oeis.org/A371596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*A000129(n) for n >= 1; a(0) = 1. A000129(n) are the
 *            Pell numbers: P(0)=0, P(1)=1, P(n) = 2*P(n-1) + P(n-2) for n >=
 *            2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */
    seq_t p0 = 0;                  /* P(0) */
    seq_t p1 = 1;                  /* P(1) */
    seq_t pn = p1;                 /* current Pell number */
    for (idx_t i = 2; i <= n; ++i) {
        pn = 2 * p1 + p0;          /* P(i) = 2*P(i-1)+P(i-2) */
        p0  = p1;
        p1  = pn;
    }
    return 4 * pn;                 /* a(n) = 4 * Pell(n) */
}
