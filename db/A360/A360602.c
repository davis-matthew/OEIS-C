/*
 * A360602  a(n) = ((2*n + 1)! / n!)^2 / (n + 1).
 * https://oeis.org/A360602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = CatalanNumber(n) * (2*n)! * (2*n + 1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   catalan, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);                /* Catalan number C_n */
    seq_t fact = factorial(2 * n);         /* (2n)!  */
    seq_t t = 2 * (seq_t)n + 1;            /* (2n+1)  */
    t = t * t;                             /* (2n+1)^2 */
    seq_t res = cat;
    res = res * fact;
    res = res * t;
    return res;
}
