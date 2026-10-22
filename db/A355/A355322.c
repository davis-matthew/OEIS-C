/*
 * A355322  LCM of Lucas numbers {L(1), L(2), ..., L(n)}.
 * https://oeis.org/A355322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(L(1), L(2), …, L(n)) – the least common multiple of
 *            the first n Lucas numbers
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   _mylgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_mylgcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; values below the offset are irrelevant. */
    if (n == 0) return 0;

    /* Handle the first Lucas numbers explicitly. */
    if (n == 1) return (seq_t)1;          /* L(1) = 1, lcm = 1 */
    if (n == 2) return (seq_t)3;          /* L(2) = 3, lcm(1,3) = 3 */

    /* Lucas recurrence: L(1)=1, L(2)=3, L(k)=L(k-1)+L(k-2) for k≥3. */
    seq_t l_prev2 = (seq_t)1;             /* L(1) */
    seq_t l_prev1 = (seq_t)3;             /* L(2) */
    seq_t lcm_val = l_prev2;              /* start with lcm = L(1) */

    /* Incorporate L(2). */
    seq_t g = _mylgcd(lcm_val, l_prev1);
    lcm_val = (lcm_val / g) * l_prev1;    /* lcm(1,3) = 3 */

    /* Loop over remaining Lucas numbers up to n. */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t l_curr = l_prev1 + l_prev2; /* Lucas recurrence */
        g = _mylgcd(lcm_val, l_curr);
        lcm_val = (lcm_val / g) * l_curr; /* update lcm */
        l_prev2 = l_prev1;
        l_prev1 = l_curr;
    }

    return lcm_val;
}
