/*
 * A312402  Coordination sequence Gal.4.70.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A312402: Coordination sequence Gal.4.70.4
   Recurrence: a(n) =  2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 5*a(n-4) + 6*a(n-5)
                - 6*a(n-6) + 6*a(n-7) - 5*a(n-8) + 4*a(n-9) - 3*a(n-10)
                + 2*a(n-11) - a(n-12)    for n > 12
   Initial values (a(0) .. a(12)):
   1, 4, 8, 14, 18, 20, 24, 28, 32, 38, 44, 48, 50
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Initial 13 terms (a(0) to a(12)) */
    static const seq_t init[13] = {
        1ULL, 4ULL, 8ULL, 14ULL, 18ULL, 20ULL, 24ULL, 28ULL,
        32ULL, 38ULL, 44ULL, 48ULL, 50ULL
    };

    if (n < 13U)
        return init[(size_t)n];

    /* last[0] = a(n-12), ..., last[11] = a(n-1) */
    seq_t last[12];
    for (size_t i = 0; i < 12; ++i)
        last[i] = init[i + 1];   /* initialize with a(1) .. a(12) */

    for (idx_t cur = 13; cur <= n; ++cur)
    {
        /* compute a(cur) using the recurrence and the values in last[] */
        seq_t next = 0;
        next = 2 * last[11]          /* a(cur-1)   */
             - 3 * last[10]          /* a(cur-2)   */
             + 4 * last[9]           /* a(cur-3)   */
             - 5 * last[8]           /* a(cur-4)   */
             + 6 * last[7]           /* a(cur-5)   */
             - 6 * last[6]           /* a(cur-6)   */
             + 6 * last[5]           /* a(cur-7)   */
             - 5 * last[4]           /* a(cur-8)   */
             + 4 * last[3]           /* a(cur-9)   */
             - 3 * last[2]           /* a(cur-10)  */
             + 2 * last[1]           /* a(cur-11)  */
             - 1 * last[0];          /* a(cur-12)  */

        /* shift the window: discard a(cur-12) */
        for (size_t i = 0; i < 11; ++i)
            last[i] = last[i + 1];
        last[11] = next;
    }

    return last[11]; /* a(n) */
}
