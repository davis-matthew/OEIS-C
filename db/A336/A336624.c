/*
 * A336624  Triangular numbers that are one-eighth of other triangular
 *          numbers; T(t) such that 8*T(t)=T(u) for some u where T(k) is the
 *          k-th triangular number.
 * https://oeis.org/A336624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1154 * a(n-2) - a(n-4) + 81 (n ≥ 4) * Initial terms:
 *            a(0)=0, a(1)=15, a(2)=66, a(3)=17391
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first four terms */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)15;
    if (n == (idx_t)2) return (seq_t)66;
    if (n == (idx_t)3) return (seq_t)17391;

    /* Keep the last four computed values:
     * v0 = a[i-4], v1 = a[i-3], v2 = a[i-2], v3 = a[i-1]  (i starts at 4) */
    seq_t v0 = (seq_t)0;          /* a(0)   */
    seq_t v1 = (seq_t)15;         /* a(1)   */
    seq_t v2 = (seq_t)66;         /* a(2)   */
    seq_t v3 = (seq_t)17391;      /* a(3)   */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        seq_t next = ((seq_t)1154 * v2 - v0) + (seq_t)81;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}
