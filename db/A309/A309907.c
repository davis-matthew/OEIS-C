/*
 * A309907  a(n) is the square of the number consisting of one 1 and n 3's:
 *          (133...3)^2.
 * https://oeis.org/A309907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((4*10^n - 1)/3)^2 Recurrence: a(n) = 111*a(n-1) -
 *            1110*a(n-2) + 1000*a(n-3), n >= 3 Initial values: a(0)=1,
 *            a(1)=169, a(2)=17689
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)169;
    if (n == 2) return (seq_t)17689;

    /* Use recurrence for n >= 3 */
    seq_t a0 = (seq_t)1;          /* a(n-3)   */
    seq_t a1 = (seq_t)169;        /* a(n-2)   */
    seq_t a2 = (seq_t)17689;      /* a(n-1)   */
    seq_t ai = (seq_t)0;          /* a(n)     */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)111 * a2 - (seq_t)1110 * a1 + (seq_t)1000 * a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return ai;
}
