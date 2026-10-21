/*
 * A290910  a(n) = (1/5)*A290909(n), n>= 0.
 * https://oeis.org/A290910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=4, a(3)=15,
   a(n)=4*a(n-1)-a(n-2)+4*a(n-3)-a(n-4)  for n >= 4 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)15;

    /* use a sliding window of the last four values */
    seq_t a0 = (seq_t)0;       /* a(n-4)   */
    seq_t a1 = (seq_t)1;       /* a(n-3)   */
    seq_t a2 = (seq_t)4;       /* a(n-2)   */
    seq_t a3 = (seq_t)15;      /* a(n-1)   */
    seq_t an;                  /* a(n)     */

    for (idx_t i = 4; i <= n; ++i) {
        an = 4 * a3 - a2 + 4 * a1 - a0;  /* compute a(i) */
        a0 = a1;                        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
