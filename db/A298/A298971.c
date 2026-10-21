/*
 * A298971  Number of compositions of n that are proper powers of Lyndon
 *          words.
 * https://oeis.org/A298971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2^d-1)*(phi(n/d)-mu(n/d))/n
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   phi_mu_from_n, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/phi_mu_from_n.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* defensive, offset is 1 */

    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {    /* iterate divisors up to sqrt(n) */
        if (n % d != 0) continue;

        /* first divisor d */
        seq_t pow_term = pow2(d);
        pow_term -= 1;                         /* 2^d - 1 */
        seq_t phi; int mu;
        phi_mu_from_n(n / d, &phi, &mu);
        seq_t diff = phi;
        if (mu >= 0) diff -= (seq_t)mu;
        else        diff += (seq_t)(-mu);
        sum += pow_term * diff;

        /* second divisor n/d, if distinct */
        idx_t d2 = n / d;
        if (d2 != d) {
            seq_t pow_term2 = pow2(d2);
            pow_term2 -= 1;
            seq_t phi2; int mu2;
            phi_mu_from_n(n / d2, &phi2, &mu2);
            seq_t diff2 = phi2;
            if (mu2 >= 0) diff2 -= (seq_t)mu2;
            else          diff2 += (seq_t)(-mu2);
            sum += pow_term2 * diff2;
        }
    }

    return sum / ((seq_t)n);     /* division is exact */
}
