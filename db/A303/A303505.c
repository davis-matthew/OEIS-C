/*
 * A303505  Number of odd chordless cycles in the n-triangular (Johnson)
 *          graph.
 * https://oeis.org/A303505
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2}^{ceil(n/2)-1} binomial(n,2*k+1)*(2*k)!/2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Small n < 5 gives 0 by definition */
    if (n < 5) return 0;

    /* k_max = floor((n-1)/2) */
    idx_t k_max = (n - 1) / 2;
    if (k_max < 2) return 0; /* safety, although handled by n < 5 */

    seq_t sum = 0;

    /* initial k = 2  =>  r = 2*k+1 = 5  */
    int r = 5;
    seq_t bin = binom(n, (idx_t)r);         /* binomial(n,5)           */
    seq_t fact = 24;                        /* (2*2)! = 4!             */
    sum += (bin * fact) / (seq_t)2;          /* add first term          */

    /* iterate for k = 3 .. k_max */
    for (idx_t k = 3; k <= k_max; ++k) {
        int r_old = r;
        int r_new = r_old + 2;               /* r = 2*k+1 after increment */

        /* update the binomial coefficient:
           bin(n, r_new) = bin(n, r_old) *
                           (n - r_old)   / (r_old + 1) *
                           (n - (r_old+1)) / r_new          */
        seq_t bin_new = bin * (n - (idx_t)r_old) / (seq_t)(r_old + 1);
        bin_new = bin_new * (n - (idx_t)(r_old + 1)) / (seq_t)r_new;
        bin = bin_new;
        r = r_new;

        /* update factorial:  (2*k)! = (2*(k-1))! * (2*(k-1)+1) * (2*(k-1)+2)   */
        fact = fact * (seq_t)(r - 2) * (seq_t)(r - 1);

        /* add term */
        sum += (bin * fact) / (seq_t)2;
    }

    return sum;
}
