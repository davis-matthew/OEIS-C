/*
 * A383900  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where column k is the expansion of Product_{j=0..k} (1 + j*x)/(1 -
 *          j*x).
 * https://oeis.org/A383900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=0..k} |Stirling1(k+1,j+1)| * Stirling2(j+n,k)
 *            where (n,k) are obtained by reading the square array A(n,k)
 *            along antidiagonals downwards
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   sum_stirling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sum_stirling.h"

seq_t a(idx_t n)
{
    return sum_stirling(n);
}
