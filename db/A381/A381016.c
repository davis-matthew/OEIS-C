/*
 * A381016  Expansion of e.g.f. -log(1-x) * sin(x).
 * https://oeis.org/A381016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{k=1..floor(n/2)} (-1)^k * (n-2*k)! *
 *            binomial(n,2*k-1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t max_r = (n - 2) / 2;
    seq_t sum   = 0;

    /* Initial values for r = 0:
       - factorial part: (n-2)!  */
    seq_t fact  = factorial(n - 2);

    /* - binomial part: binom(n,1) = n */
    seq_t binom = (seq_t)n;

    for (idx_t r = 0; r <= max_r; ++r)
    {
        seq_t term = binom * fact;
        if (r & 1)   /* odd r: subtract */
            sum -= term;
        else
            sum += term;

        if (r == max_r)
            break;

        /* Update factorial part: divide by (n-2r-2)*(n-2r-3) */
        idx_t d1 = n - 2 * r - 2;
        idx_t d2 = n - 2 * r - 3;
        fact = fact / ((seq_t)(d1 * d2));

        /* Update binomial part:
           binom(n, 2(r+1)+1) = binom(n, 2r+3) */
        idx_t up1   = n - (2 * r + 1);
        idx_t up2   = n - (2 * r + 2);   /* = n - 2r - 2 */
        idx_t down1 = 2 * r + 2;
        idx_t down2 = 2 * r + 3;
        binom = binom * (seq_t)up1 * (seq_t)up2 /
                ((seq_t)down1 * (seq_t)down2);
    }

    return sum;
}
