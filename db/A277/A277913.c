/*
 * A277913  Nonsquare numbers n for which the smallest y>0 solution of the
 *          Pellian equation x^2 - n*y^2 = 1 divides n.
 * https://oeis.org/A277913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonsquare integer n for which the smallest y>0
 *            solution of x^2 - n*y^2 = 1 divides n. The sequence begins
 *            2,3,6,8,12,…
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static bool satisfies(seq_t m)
{
    seq_t sqrt_m = floor_sqrt(m);
    if (sqrt_m * sqrt_m == m)                /* skip squares       */
        return false;

    /* Continued‑fraction expansion of √m */
    seq_t a0     = sqrt_m;
    seq_t m_old  = 0;
    seq_t d_old  = 1;
    seq_t a_cur  = a0;

    /* Convergents:  p[-1]=1, p[0]=a0, q[-1]=0, q[0]=1 */
    seq_t p_prev2 = 1;
    seq_t p_prev1 = a0;
    seq_t q_prev2 = 0;
    seq_t q_prev1 = 1;

    if (p_prev1 * p_prev1 - m * q_prev1 * q_prev1 == 1) {
        return (m % q_prev1 == 0);
    }

    for (;;) {
        seq_t m_next = d_old * a_cur - m_old;
        seq_t d_next = (m - m_next * m_next) / d_old;
        seq_t a_next = (a0 + m_next) / d_next;

        seq_t p_curr = a_next * p_prev1 + p_prev2;
        seq_t q_curr = a_next * q_prev1 + q_prev2;

        if (p_curr * p_curr - m * q_curr * q_curr == 1) {
            return (m % q_curr == 0);
        }

        /* If the current y exceeds m it can never divide m */
        if (q_curr > m)
            return false;

        /* Advance indices for the next iteration */
        m_old   = m_next;
        d_old   = d_next;
        a_cur   = a_next;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but safe guard */
        return 0;

    idx_t count = 0;
    seq_t value = 2;                /* candidates start at 2 */

    for (;;) {
        if (satisfies(value)) {
            count++;
            if (count == n)
                return value;
        }
        ++value;
    }
}
