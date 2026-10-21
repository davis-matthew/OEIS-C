/*
 * A293499  Number of unlabeled hereditary semiorders on n points.
 * https://oeis.org/A293499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=42,
   a(n)=8*a(n-1)-23*a(n-2)+29*a(n-3)-14*a(n-4)+a(n-5)  for n>=6 */

seq_t a(idx_t n)
{
    static const seq_t init[5] = { (seq_t)1, (seq_t)2, (seq_t)5,
                                  (seq_t)14, (seq_t)42 };

    if (n == 0) return (seq_t)0;          /* offset 1, so this case is unused */
    if (n <= 5) return init[n-1];

    seq_t buf[5];
    for (idx_t i = 0; i < 5; ++i) buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) using previous five values */
        seq_t pos = (seq_t)8 * buf[4] + (seq_t)29 * buf[2] + buf[0];
        seq_t neg = (seq_t)23 * buf[3] + (seq_t)14 * buf[1];
        seq_t next = pos - neg;

        /* shift window: buf[0]..buf[4] become a(i-4)..a(i)  */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }

    return buf[4];
}
