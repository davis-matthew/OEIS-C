/*
 * A250594  Number of (n+2)X(1+2) 0..2 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( C(floor((n+1)/3)+2, 2) * C(floor(n/3)+2, 2) * *
 *            C(floor((n-1)/3)+2, 2) )^3 * * where C(x, 2) = x*(x-1)/2 * *
 *            This follows from the observation that, for an (n+2)×3 array
 *            with * entries 0..2, the only constraints are a_i <= a_{i+3} for
 *            * i = 1 .. n-1 in each column. Thus each residue clas...
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 43, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* lengths of residue chains */
    idx_t k1 = (n + 1) / 3 + 1;   /* positions 1,4,7,... */
    idx_t k2 = n / 3 + 1;         /* positions 2,5,8,... */
    idx_t k3 = (n - 1) / 3 + 1;   /* positions 3,6,9,... */

    /* binomial coefficient C(k+2, 2) = (k+2)(k+1)/2 */
    seq_t t1 = ((seq_t)k1 + 2) * ((seq_t)k1 + 1) / 2;
    seq_t t2 = ((seq_t)k2 + 2) * ((seq_t)k2 + 1) / 2;
    seq_t t3 = ((seq_t)k3 + 2) * ((seq_t)k3 + 1) / 2;

    seq_t prod = t1 * t2 * t3;          /* number per column */
    return prod * prod * prod;          /* cube for 3 independent columns */
}
