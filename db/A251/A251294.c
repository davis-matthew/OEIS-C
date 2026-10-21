/*
 * A251294  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 3 6 or 9.
 * https://oeis.org/A251294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 27*a(n-1) - 260*a(n-2) + 970*a(n-3) - 405*a(n-4)
   - 3449*a(n-5) - 722*a(n-6) + 840*a(n-7)
   initial values: a(1)=444, a(2)=3140, a(3)=22536, a(4)=164480,
                   a(5)=1221748, a(6)=9240612, a(7)=71163896 */

seq_t a(idx_t n)
{
    /* base values for n = 1..7 */
    static const seq_t init[7] = {
        (seq_t)444,        (seq_t)3140,
        (seq_t)22536,      (seq_t)164480,
        (seq_t)1221748,    (seq_t)9240612,
        (seq_t)71163896
    };

    if (n < 1) return (seq_t)0;      /* not part of the sequence, but defined for safety */
    if (n <= 7) return init[n - 1];

    /* constants used in the recurrence */
    static const seq_t C27   = 27,  C260  = 260,  C970   = 970,
                       C405  = 405, C3449 = 3449, C722   = 722,
                       C840  = 840;

    /* buffer holding the last 7 computed terms */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i) buf[i] = init[i];

    for (idx_t k = 8; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        sum += C27   * buf[6];
        sum -= C260  * buf[5];
        sum += C970  * buf[4];
        sum -= C405  * buf[3];
        sum -= C3449 * buf[2];
        sum -= C722  * buf[1];
        sum += C840  * buf[0];

        /* shift the window and store the new term */
        for (idx_t i = 0; i < 6; ++i) buf[i] = buf[i + 1];
        buf[6] = sum;
    }

    return buf[6];
}
