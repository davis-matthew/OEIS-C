/*
 * A387752  a(n) = (1/9) * Sum_{k=0..n-1} binomial(9*n,9*k+1).
 * https://oeis.org/A387752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/9)*Σ_{k=0}^{n-1} binomial(9*n, 9*k+1) Recurrence
 *            (valid for n>4): a(n) = 265*a(n-1) + 139823*a(n-2) -
 *            6826204*a(n-3) - 6965249*a(n-4) + 512*a(n-5)
 *            (a(0)=0,a(1)=1,a(2)=4864,a(3)=1184043,a(4)=986882648)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4864;
    if (n == 3) return (seq_t)1184043;
    if (n == 4) return (seq_t)986882648;

    seq_t buf[5] = { (seq_t)0, (seq_t)1, (seq_t)4864,
                     (seq_t)1184043, (seq_t)986882648 };

    for (idx_t i = 5; i <= n; ++i) {
        seq_t a5 = buf[0]; /* a(n-5) */
        seq_t a4 = buf[1]; /* a(n-4) */
        seq_t a3 = buf[2]; /* a(n-3) */
        seq_t a2 = buf[3]; /* a(n-2) */
        seq_t a1 = buf[4]; /* a(n-1) */

        /* compute a(n) using the recurrence */
        seq_t pos = (seq_t)265 * a1 + (seq_t)139823 * a2 + (seq_t)512 * a5;
        seq_t neg = (seq_t)6826204 * a3 + (seq_t)6965249 * a4;
        seq_t next = pos - neg; /* safe: pos > neg for n>=5 */

        /* shift buffer left by one: a(n-4) .. a(n) */
        buf[0] = a4;
        buf[1] = a3;
        buf[2] = a2;
        buf[3] = a1;
        buf[4] = next;
    }
    return buf[4];
}
