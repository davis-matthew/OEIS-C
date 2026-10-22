/*
 * A323767  A(n,k) = Sum_{j=0..floor(n/2)} binomial(n-j,j)^k, square array
 *          A(n,k) read by antidiagonals, for n >= 0, k >= 0.
 * https://oeis.org/A323767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,k) where A(i,k)=Sum_{j=0..⌊i/2⌋} binomial(i-j,j)^k,
 *            read by antidiagonals of the square array with rows n and
 *            columns k
 * Range:     u8 n <= 49, u16 n <= 84, u32 n <= 159, u64 n <= 264,
 *            u128 n <= 509, big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

/* Return the nth term of OEIS sequence A323767 */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that (d+1)(d+2)/2 > n.
       Each diagonal d has d+1 terms. */
    idx_t d = 0;
    /* cum holds cumulative number of terms up to and including diagonal d */
    idx_t cum = 1;                /* (0+1)(0+2)/2 = 1  */
    while (cum <= n)
    {
        d++;
        cum = (d + 1) * (d + 2) / 2;
    }

    /* Number of terms before diagonal d */
    idx_t cum_prev = cum - (d + 1);          /* d(d+1)/2 */

    /* Position within diagonal d (0‑based) */
    idx_t pos = n - cum_prev;

    /* Coordinates in the array: i = row = pos, k = column = d - pos */
    idx_t i = pos;
    idx_t k = d - pos;

    /* Compute A(i,k) = Σ_{t=0..⌊i/2⌋} binomial(i-t, t)^k */
    seq_t sum = 0;
    idx_t limit = i >> 1;  /* floor(i/2) */
    for (idx_t t = 0; t <= limit; ++t)
    {
        seq_t bin = binomial(i - t, t);  /* binomial coefficient (i-t choose t) */
        seq_t pow = pow_int(bin, k);     /* bin^k */
        sum += pow;
    }

    return sum;
}
