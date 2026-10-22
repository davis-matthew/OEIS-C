/*
 * A368649  a(n) = round(n*rho), where rho = (5+sqrt(5))/2.
 * https://oeis.org/A368649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*(5+sqrt(5))/2) = floor((5n + n*sqrt(5))/2 + 1/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Compute floor(n*sqrt(5)) as floor(sqrt(5*n*n)) */
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn;   /* n^2 */
    t *= (seq_t)5;        /* 5*n^2 */
    seq_t s = floor_sqrt(t);  /* floor(n*sqrt(5)) */

    /* Compute a(n) = (5*n + s)/2 rounded */
    seq_t t2 = (seq_t)5 * nn + s;    /* 5n + floor(n*sqrt(5)) */
    return (t2 >> 1) + (t2 & (seq_t)1);   /* (t2/2) + (t2 odd ? 1 : 0) */
}
