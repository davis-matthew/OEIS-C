/*
 * A294445  a(n) = (2*n + 4)!*(n^2 + 11*n + 2)/(2*(n-1)!*(n+6)!).
 * https://oeis.org/A294445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n + 4)!*(n^2 + 11*n + 2)/(2*(n-1)!*(n+6)!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1; guard for n<1 */

    idx_t N = 2 * n + 4;                 /* upper limit of factorial */
    idx_t K = n - 1;                     /* lower part of binomial   */
    seq_t binom = 1;

    /* Compute binomial coefficient C(2n+4, n-1) multiplicatively,
       cancelling factors with the denominator on the fly. */
    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i);  /* numerator part of the i-th factor */
        seq_t den = (seq_t)i;            /* denominator part (i) */

        /* cancel any common factors between current binom result and den */
        seq_t g = gcd(binom, den);
        binom = binom / g;
        den   = den   / g;

        /* cancel any remaining common factors between num and den */
        g = gcd(num, den);
        num = num / g;
        den = den / g;   /* den should now be 1 */

        binom = binom * num;
    }

    /* polynomial factor */
    seq_t term = (seq_t)n * (seq_t)n + (seq_t)11 * (seq_t)n + (seq_t)2;

    /* denominator factor 2*(n+6) */
    seq_t denom = (seq_t)2 * ((seq_t)n + (seq_t)6);

    /* cancel common factors between term and denom first */
    seq_t g = gcd(term, denom);
    term = term / g;
    denom = denom / g;

    /* then cancel any remaining factors between binom and denom */
    g = gcd(binom, denom);
    binom = binom / g;
    denom = denom / g; /* denom should now be 1 */

    /* final result: binom * term (denom == 1) */
    return binom * term;
}
