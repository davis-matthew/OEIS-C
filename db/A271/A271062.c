/*
 * A271062  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 261", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A271062
    Formula: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) - 3*a(n-4) 
                            + 3*a(n-5) + a(n-6) - a(n-7)   for n > 6
    Initial values: a(0)=1, a(1)=9, a(2)=10, a(3)=58, a(4)=59, a(5)=179, a(6)=180.
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base values for 0 ≤ n ≤ 6 */
    static const seq_t init[7] = { (seq_t)1,
                                   (seq_t)9,
                                   (seq_t)10,
                                   (seq_t)58,
                                   (seq_t)59,
                                   (seq_t)179,
                                   (seq_t)180 };

    if (n < (idx_t)7)
        return init[(size_t)n];

    /* buffer holding the last seven values needed for the recurrence */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    /* compute a(n) iteratively */
    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t next = buf[6]                       /* a(i-1) */
                  + buf[5] * (seq_t)3              /* + 3*a(i-2) */
                  - buf[4] * (seq_t)3              /* - 3*a(i-3) */
                  - buf[3] * (seq_t)3              /* - 3*a(i-4) */
                  + buf[2] * (seq_t)3              /* + 3*a(i-5) */
                  + buf[1]                        /* + a(i-6) */
                  - buf[0];                       /* - a(i-7) */

        /* shift buffer for next step */
        for (size_t j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = next;
    }

    return buf[6];
}
