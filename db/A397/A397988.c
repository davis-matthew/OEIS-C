/*
 * A397988  Lower (1/3,1/2) midsequence of triangular numbers (A000217) and
 *          squares (A000290).
 * https://oeis.org/A397988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n>=6, with
 *            a(0)=0,a(1)=0,a(2)=3,a(3)=6,a(4)=11,a(5)=17
 * Range:     u8 n <= 19, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6) {
        static const seq_t vals[6] = {0, 0, 3, 6, 11, 17};
        return vals[n];
    }
    seq_t buf[6];
    buf[0] = 0;          /* a0   */
    buf[1] = 0;          /* a1   */
    buf[2] = 3;          /* a2   */
    buf[3] = 6;          /* a3   */
    buf[4] = 11;         /* a4   */
    buf[5] = 17;         /* a5   */

    for (idx_t i = 6; i <= n; ++i) {
        /* a_i = a_{i-1} + a_{i-2} - a_{i-4} - a_{i-5} + a_{i-6} */
        seq_t next = buf[5] + buf[4] + buf[0];
        next = next - buf[2] - buf[1];

        /* shift buffer left */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }
    return buf[5];
}
