/*
 * A349975  Expansion of g.f. (x^4*(x^2 + 2*x + 3))/((x - 1)^4*(x + 1)*(x^2 +
 *          x + 1)).
 * https://oeis.org/A349975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=a(3)=0, a(4)=3, a(5)=8, a(6)=17;
   for n≥7, a(n)=2*a(n-1)-a(n-3)-a(n-4)+2*a(n-6)-a(n-7). */
seq_t a(idx_t n)
{
    static const seq_t init[7] = {0, 0, 0, 0, 3, 8, 17};
    if (n < 7) return init[n];

    seq_t buf[7];
    for (int i = 0; i < 7; ++i) buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t next = buf[6] + buf[6];           /* 2*a(n-1)   */
        next -= buf[4];                        /* -a(n-3)    */
        next -= buf[3];                        /* -a(n-4)    */
        next += buf[1] + buf[1];               /* +2*a(n-6)  */
        next -= buf[0];                        /* -a(n-7)    */

        for (int j = 0; j < 6; ++j) buf[j] = buf[j + 1];
        buf[6] = next;
    }
    return buf[6];
}
