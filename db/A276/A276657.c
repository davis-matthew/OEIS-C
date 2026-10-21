/*
 * A276657  Number of ways to occupy n unlabeled phone booths in a circle one
 *          by one, each time picking a phone booth adjacent to the smallest
 *          number of previously occupied phone booths.
 * https://oeis.org/A276657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{2m+3k=n} (m+k-1)! * C(m+k,m) * 2^k * k! * (m+k)!
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 25, u128 n <= 41,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base case: a(1) = 1 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t sum = 0;

    for (idx_t k = 0; 3 * k <= n; ++k) {
        idx_t rem = n - 3 * k;           /* remaining to be covered by 2m */
        if (rem & 1) continue;           /* must be even */
        idx_t m  = rem >> 1;             /* m = rem / 2 */
        idx_t mk = m + k;                /* m + k */

        /* Compute the term efficiently: (mk-1)! * (mk!)^2 * 2^k / m! */
        seq_t term = factorial(mk);                    /* mk! */
        term /= factorial(m);                          /* divide by m!   */
        term *= factorial(mk - 1);                     /* * (mk-1)!      */
        term *= factorial(mk);                         /* * mk!          */
        term *= pow2(k);                               /* * 2^k          */

        sum += term;
    }

    return sum;
}
