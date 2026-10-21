/*
 * A282634  Recursive 2-parameter sequence allowing the Ramanujan's sum
 *          calculation.
 * https://oeis.org/A282634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th term of the row‑wise listing of the Ramanujan
 *            sum table c_q(t) = Sum_{d|gcd(q,t)} d·μ(q/d), where μ is the
 *            Möbius function
 * Range:     i8 n <= 7395, i16 n <= 9870, i32 n <= 9870, i64 n <= 9870,
 *            i128 n <= 9870, big any n
 * Helpers:   gcd, mobius, upper_bound_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mobius.h"
#include "../utils/upper_bound_row.h"

seq_t a(idx_t i)
{
    /* Convert the 1‑based flat index i to the position (n,t) in the
       two‑parameter array. */
    idx_t n = upper_bound_row(i);
    idx_t prev = (n - 1) * n / 2;          /* number of terms before row n */
    idx_t t   = i - prev - 1;              /* column index, 0‑based */

    /* Compute the Ramanujan sum c_n(t) = Sum_{d|gcd(n,t)} d·μ(n/d) */
    seq_t g_seq = gcd( (seq_t)n, (seq_t)t );   /* use seq_t version of gcd */
    idx_t g = (idx_t)g_seq;                     /* g fits in idx_t */

    seq_t sum = 0;
    for (idx_t d = 1; d <= g / d; ++d)
    {
        if (g % d == 0)
        {
            seq_t mu1 = mobius( n / d );        /* μ(n/d) */
            sum += (seq_t)d * mu1;

            idx_t d2 = g / d;
            if (d2 != d)
            {
                seq_t mu2 = mobius( n / d2 );   /* μ(n/d2) */
                sum += (seq_t)d2 * mu2;
            }
        }
    }
    return sum;
}
