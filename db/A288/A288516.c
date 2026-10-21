/*
 * A288516  Number of (undirected) paths in the ladder graph P_2 X P_n.
 * https://oeis.org/A288516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*(2^n - 1) - n*(n^2 + 9*n + 41)/3. (Equivalently: a(n)
 *            = 18*(2^n - 1) - (41*n)/3 - 3*n^2 - n^3/3.)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, return 0 */

    seq_t nn = n;                         /* promote index to seq_t */
    seq_t pow2_val = pow2(n);                 /* 2^n */
    seq_t term1 = (seq_t)18 * (pow2_val - (seq_t)1);

    seq_t n_sq = nn * nn;
    seq_t numer = nn * (n_sq + (seq_t)9 * nn + (seq_t)41);
    seq_t term2 = numer / (seq_t)3;           /* division is exact */

    return term1 - term2;
}
