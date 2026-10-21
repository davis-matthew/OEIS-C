/*
 * A293643  a(n) is the least integer k such that k/Fibonacci(n) > 3/5.
 * https://oeis.org/A293643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(3 * Fibonacci(n) / 5) The following implementation
 *            uses only addition and avoids any overflow by maintaining the
 *            Fibonacci number in the form F(n) = 5*q(n) + r(n) with 0 <= r(n)
 *            < 5. It then computes a(n) from q(n) and r(n)
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial Fibonacci terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* q(k) = floor(F(k)/5),   r(k) = F(k) mod 5   for k = 0,1          */
    seq_t q_prev2 = (seq_t)0;          /* q(0)   */
    seq_t q_prev  = (seq_t)0;          /* q(1)   */
    uint8_t r_prev2 = 0;               /* r(0)   */
    uint8_t r_prev  = 1;               /* r(1)   */

    /* a(0) and a(1) for reference */
    seq_t a_prev2 = (seq_t)0;
    seq_t a_prev  = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute q(i) and r(i) from q(i-1),r(i-1) and q(i-2),r(i-2) */
        uint8_t r_sum = r_prev + r_prev2;
        seq_t  q_sum = q_prev + q_prev2;
        if (r_sum >= 5) {
            r_sum -= 5;
            q_sum += (seq_t)1;
        }

        /* a(i) = ceil(3 * F(i) / 5) = 3*q_sum + ceil(3*r_sum/5) */
        seq_t a_val = q_sum + q_sum + q_sum; /* 3 * q_sum   */
        uint8_t small = (3u * r_sum + 4) / 5; /* ceil(3*r_sum/5) */
        a_val += (seq_t)small;

        /* Shift for next iteration */
        q_prev2 = q_prev;
        q_prev  = q_sum;
        r_prev2 = r_prev;
        r_prev  = r_sum;
        a_prev2 = a_prev;
        a_prev  = a_val;
    }

    return a_prev;
}
