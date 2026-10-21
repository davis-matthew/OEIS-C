/*
 * A245678  Denominator of sum of fractions A182972(k) / A182973(k) such that
 *          A182972(k) + A182973(k) = n.
 * https://oeis.org/A245678
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the reduced sum Σ_{1 ≤ d < n/2,
 *            gcd(d,n)=1} d/(n-d) The sum consists of all reduced fractions
 *            p/q < 1 with p+q = n, i.e. p = d, q = n − d. The fractions are
 *            summed as normal rational numbers; the result is then put in
 *            lowest terms and its denominator is returned. For n < 3 t...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 28, u64 n <= 48, u128 n <= 100,
 *            big any n
 * Helpers:   gcd, local_gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
/* gcd for seq_t values */
#include "../utils/gcd.h"
#include "../utils/local_gcd_int.h"

seq_t a(idx_t n)
{
    if (n < 3)     /* value not defined for n < 3 */
        return 0;

    /* half = floor((n-1)/2) gives the largest d such that d < n/2 */
    idx_t half = (n - 1) / 2;
    idx_t d;

    /* First pass: compute the least common multiple L of all denominators q = n-d */
    seq_t L = 1;
    for (d = 1; d <= half; ++d) {
        if (local_gcd_int((int)d, (int)n) != 1)
            continue;
        idx_t q = n - d;                /* q > n/2 */
        seq_t qseq = (seq_t)q;
        seq_t g = gcd(L, qseq);
        L = (L / g) * qseq;
    }

    /* Second pass: compute the numerator N of the sum in terms of L */
    seq_t N = 0;
    for (d = 1; d <= half; ++d) {
        if (local_gcd_int((int)d, (int)n) != 1)
            continue;
        idx_t q = n - d;
        seq_t factor = L / (seq_t)q;    /* exact division: q | L */
        N += (seq_t)d * factor;
    }

    /* Reduce the fraction N/L and return the denominator */
    seq_t g = gcd(N, L);
    seq_t denom = L / g;
    return denom;
}
