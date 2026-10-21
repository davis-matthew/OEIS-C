/*
 * A258201  a(n) = smallest number encountered when iterating the map x ->
 *          floor(tan(x)) starting from the initial value x = n.
 * https://oeis.org/A258201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the smallest number encountered when iterating x ->
 *            floor(tan(x)) starting from x = n. The iteration always
 *            terminates at the fixed points 0 or 1
 * Range:     i8 n <= 10, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_tan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/floor_tan.h"

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;     /* current value of the iteration   */
    seq_t m   = cur;          /* current minimum found so far      */

    while (1) {
        seq_t nxt = floor_tan((long long)cur);   /* floor(tan(cur))  */

        if (nxt == cur) { /* fixed point reached */
            /* cur is 0 or 1, so return the minimum found. */
            return (m < cur ? m : cur);
        }

        if (nxt < m)          /* update minimum if necessary */
            m = nxt;

        cur = nxt;            /* continue the iteration */
    }
}
