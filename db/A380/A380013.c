/*
 * A380013  Continued fraction expansion of Sum_{i>=0} (-1)^i/(q(i)*q(i+1))
 *          where q(0)=q(1)=1, q(2n+2)=q(2n+1)+q(2n), and
 *          q(2n+3)=q(2n+1)*(q(2n+2)+1).
 * https://oeis.org/A380013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(2*n) = 1 (n ≥ 1), a(2*n+1) = q(2*n-1) with q(0) = q(1) = 1,
 *            q(2n+2) = q(2n+1) + q(2n), q(2n+3) = q(2n+1) * (q(2n+2) + 1)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t q[10001] = { (seq_t)1, (seq_t)1 };   /* q[0] = q[1] = 1   */
static idx_t q_len = 2;        /* number of q values computed */

static void ensure_q(idx_t need)
/* Ensure that q[0..need] are computed (need ≥ 0).   */
{
    if (need < q_len) return;
    for (idx_t i = q_len; i <= need; ++i) {
        if ((i & 1) == 0)          /* even index:   q[i] = q[i-1] + q[i-2]   */
            q[i] = q[i-1] + q[i-2];
        else                        /* odd  index:   q[i] = q[i-2] * (q[i-1] + 1) */
            q[i] = q[i-2] * (q[i-1] + (seq_t)1);
    }
    q_len = need + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */
    if ((n & 1) == 0) return (seq_t)1;    /* even indices ≥ 2 */
    /* odd index: a(n) = q(n-2) */
    ensure_q(n - 2);
    return q[n - 2];
}
