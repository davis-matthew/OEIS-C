/*
 * A258772  Number of fixed points in the Collatz (3x+1) trajectory of n.
 * https://oeis.org/A258772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count fixed points in the Collatz trajectory of n.
   A fixed point is a term T_k that satisfies T_k == k
   (1‑based indices). The trajectory stops when n reaches 1,
   because subsequent terms (4, 2, 1, …) can never satisfy
   T_k == k again.  */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If n < 1 (outside OEIS offset) the result is undefined;
       we simply return 0.                                             */
    if (n < 1) return 0;

    seq_t cur = (seq_t)n;      /* current value of the trajectory   */
    idx_t  step = 1;           /* position in the trajectory (1‑based) */
    seq_t  count = 0;          /* number of fixed points            */

    while (1)
    {
        if (cur == (seq_t)step) ++count;   /* fixed point found          */
        if (cur == (seq_t)1) break;        /* end of useful trajectory    */

        /* Next Collatz step                                          */
        if ((cur & (seq_t)1) == 0)          /* even number                */
            cur = cur >> 1;
        else
            cur = cur * (seq_t)3 + (seq_t)1;

        ++step;
    }

    return count;
}
