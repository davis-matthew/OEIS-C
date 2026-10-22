/*
 * A350526  a(n) = f(n*r)*c(n/r), where f = floor, c = ceiling, and r = golden
 *          ratio (A001622).
 * https://oeis.org/A350526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r) * ceil(n/r), r = golden ratio (1+sqrt5)/2
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; n>=1 in usual use */

    seq_t nSeq = (seq_t)n;
    seq_t nsq   = nSeq * nSeq;            /* n^2  */
    seq_t s     = floor_sqrt((seq_t)5 * nsq); /* floor(sqrt(5*n^2)) = floor(n*sqrt5) */

    seq_t floor_n_phi = (nSeq + s) / 2;        /* floor(n*phi)   */
    seq_t ceil_n_phi  = (s - nSeq) / 2 + 1;    /* ceil(n/phi)    */

    return floor_n_phi * ceil_n_phi;
}
