/*
 * A382088  Expansion of e.g.f. (1/x) * Series_Reversion( x * exp(-x * B(x)^3)
 *          ), where B(x) = 1 + x*B(x)^4 is the g.f. of A002293.
 * https://oeis.org/A382088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A382088
 *  Formula: a(n) = (n-1)! * Σ_{k=0}^{n-1} (n+1)^(n-k-1) * binomial(3*n + k -1 , k)/(n-k-1)!
 *  (a(0)=1)
 *  This implementation evaluates the sum directly in O(n) time for each n.
 */

#include <stddef.h>      /* for size_t */
#include <stdint.h>      /* for uint64_t, etc. */

/*  The helper binomial routine is assumed to be available from utils/binomial.h */
#include "../utils/binomial.h"

/*  a(n) as defined in OEIS entry A382088 */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* n >= 1 */
    seq_t inc = (seq_t)(n + 1);          /* (n+1) used repeatedly */
    seq_t power = (seq_t)1;              /* (n+1)^(n-1) starts here */

    /* compute power = (n+1)^(n-1) */
    for (idx_t i = 0; i < n - 1; ++i) {
        power *= inc;
    }

    seq_t sum = (seq_t)0;          /* accumulator for the sum */
    seq_t factor = (seq_t)1;       /* (n-1)! / (n-k-1)! part, starts at 1 (k=0) */
    seq_t binom = (seq_t)1;        /* binomial(3*n + k -1 , k), starts at 1 (k=0) */

    for (idx_t k = 0; k < n; ++k) {
        /* term = power * binom * factor  (all integers) */
        seq_t term = power * binom * factor;
        sum += term;

        /* update for next k (if any remaining) */
        if (k + 1 < n) {
            /* factor ← factor * (n - k - 1)     (adds next multiplier to factorial) */
            factor *= (seq_t)(n - k - 1);

            /* binom ← binom * (3*n + k) / (k + 1)   (next binomial value) */
            binom = (binom * (seq_t)(3 * n + k)) / (seq_t)(k + 1);

            /* power ← power / (n+1)   (next power exponent) */
            power /= inc;
        }
    }

    return sum;
}
