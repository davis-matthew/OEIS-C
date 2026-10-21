/*
 * A276862  First differences of the Beatty sequence A003151 for 1 + sqrt(2).
 * https://oeis.org/A276862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(1+sqrt(2))) - floor(n*(1+sqrt(2)))
 * Range:     u8 n <= 4, u16 n <= 678, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = 1 + floor(sqrt(2*(n+1)^2)) - floor(sqrt(2*n^2)) */

/* Needs the helper function floor_sqrt from "../utils/floor_sqrt.h". */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) {
        /* offset starts at 1; behavior for n==0 is irrelevant */
        return 0;
    }
    seq_t nn = (seq_t)n;                    /* cast index to seq_t */
    seq_t t1 = nn * nn;                    /* n^2 */
    t1 = t1 + t1;                           /* 2 * n^2 */
    seq_t t2 = nn + (seq_t)1;               /* n+1 */
    t2 = t2 * t2;                           /* (n+1)^2 */
    t2 = t2 + t2;                           /* 2 * (n+1)^2 */
    seq_t f1 = floor_sqrt(t1);              /* floor(sqrt(2*n^2)) */
    seq_t f2 = floor_sqrt(t2);              /* floor(sqrt(2*(n+1)^2)) */
    return (seq_t)1 + (f2 - f1);
}
