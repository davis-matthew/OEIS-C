/*
 * A368928  Triangle read by rows where T(n,k) is the number of labeled
 *          loop-graphs with n vertices and n edges, k of which are loops.
 * https://oeis.org/A368928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8*n+1)−1)/2⌋, k = n − r*(r+1)/2,
 *            T(r,k) = C(r,k) * C(C(r,2), r−k) (C = binomial)
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 350,
 *            big any n
 * Helpers:   binom_seq_big, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binom_seq_big.h"

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8*n+1)-1)/2) */
    seq_t t = (seq_t)8 * (seq_t)n + (seq_t)1;
    seq_t s = floor_sqrt(t);          /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((s - (seq_t)1) / (seq_t)2);

    /* k = n - r*(r+1)/2 */
    seq_t base = (seq_t)r * ((seq_t)r + (seq_t)1) / (seq_t)2;
    seq_t kseq = (seq_t)n - base;
    idx_t k = (idx_t)kseq;

    /* left part: C(r,k) */
    seq_t left = binom_seq_big((seq_t)r, k);

    /* middle part: C(r,2) */
    seq_t choose2 = (seq_t)0;
    if (r >= 2)
        choose2 = (seq_t)r * ((seq_t)r - (seq_t)1) / (seq_t)2;

    /* right part: C(C(r,2), r-k) */
    seq_t right = binom_seq_big(choose2, (idx_t)(r - k));

    return left * right;
}
