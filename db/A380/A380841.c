/*
 * A380841  Array read by ascending antidiagonals: A(n,k) = n! * [x^n] 1/(1 -
 *          x*exp(x))^k.
 * https://oeis.org/A380841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = n! * Sum_{j=0..n} j^(n-j) * C(j+k-1,j)/(n-j)!
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): read the array A(i,j) by ascending antidiagonals. */

#include <stdint.h>
#include "../utils/pow_seq.h"

static seq_t compute_A(idx_t n, idx_t k)
{
    if (n == 0u)          /* row 0: all values are 1 */
        return (seq_t)1;
    if (k == 0u)          /* for n>0 and k==0 all entries are 0 */
        return (seq_t)0;

    seq_t result = (seq_t)0;

    /* we iterate over j = 1 .. n, the j=0 term is zero for n>0 */
    idx_t j = 1u;
    seq_t P = (seq_t)n;                         /* P(n,1) = n!/(n-1)! = n */
    seq_t B = (seq_t)k;                         /* C(k,1) = k */
    seq_t jpow = pow_seq((seq_t)j, (int)(n - j));/* j^(n-j) */

    seq_t term = P * jpow * B;
    result += term;

    for (j = 1u; j < n; ++j) {
        /* update to next j+1 */
        P = P * (seq_t)(n - j);                 /* P(n,j+1) = P(n,j)*(n-j) */

        /* B_{j+1} = B_j * (k+j) / (j+1) */
        seq_t numer = B * (seq_t)(k + j);
        B = numer / (seq_t)(j + 1);

        /* compute (j+1)^(n-(j+1)) */
        jpow = pow_seq((seq_t)(j + 1), (int)(n - (j + 1)));

        term = P * jpow * B;
        result += term;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0u;
    while (s * (s + 1u) / 2u <= n)
        ++s;
    --s;  /* now s is the diagonal containing index n */

    /* offset within the diagonal */
    idx_t offset = n - s * (s + 1u) / 2u;
    idx_t i = s - offset;   /* row index */
    idx_t j = offset;       /* column index */

    return compute_A(i, j);
}
