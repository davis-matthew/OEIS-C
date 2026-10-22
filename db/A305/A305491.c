/*
 * A305491  a(n) = numerator(r(n)) where r(n) = (((1/2)*(sqrt(3) + 1))^n -
 *          ((1/2)*(sqrt(3) - 1))^n * cos(Pi*n))/sqrt(3).
 * https://oeis.org/A305491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator(r(n)) where * r(n) = (((1/2)*(sqrt(3)+1))^n -
 *            ((1/2)*(sqrt(3)-1))^n * cos(Pi*n))/sqrt(3) * and a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=3, ... (OEIS A305491)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the integer sequence using only integer arithmetic.
 * Let u = sqrt(3)+1, v = sqrt(3)-1.
 * Then (u^n) = (p_u + q_u*sqrt(3))/2^n
 *      (v^n) = (p_v + q_v*sqrt(3))/2^n
 * The recurrence for u and v is:
 *   p_{k+1} = p_k + 3*q_k          q_{k+1} = p_k + q_k
 *   p'_{k+1} = 3*q_k - p_k        q'_{k+1} = p_k - q_k
 * Using these we get
 *   r(n) = (q_u - (-1)^n q_v) / 2^n
 * The numerator a(n) is obtained by canceling common powers of 2.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* u = sqrt(3)+1 */
    /* v = sqrt(3)-1 */
    seq_t pu = (seq_t)1, qu = (seq_t)0;
    seq_t pv = (seq_t)1, qv = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* update u */
        seq_t new_pu = pu + (seq_t)3 * qu;
        seq_t new_qu = pu + qu;
        pu = new_pu;
        qu = new_qu;

        /* update v */
        seq_t new_pv = (seq_t)3 * qv - pv;
        seq_t new_qv = pv - qv;
        pv = new_pv;
        qv = new_qv;
    }

    /* Q = qu - (-1)^n * qv */
    seq_t Q;
    if (n & 1)          /* n odd: (-1)^n = -1 */
        Q = qu + qv;
    else                /* n even: (-1)^n = +1 */
        Q = qu - qv;

    /* if Q is zero, the result is zero */
    if (Q == (seq_t)0) return (seq_t)0;

    /* compute v2(Q) = exponent of 2 dividing Q */
    idx_t v2 = 0;
    seq_t tmp = Q;
    while ((tmp & (seq_t)1) == (seq_t)0)
    {
        tmp >>= 1;
        ++v2;
        /* if we have already shifted n times we can stop early */
        if (v2 == n) break;
    }
    if (v2 > n) v2 = n; /* only cancel up to denominator's power */

    return Q >> v2;
}
