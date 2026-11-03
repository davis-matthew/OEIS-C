/*
 * A341523  Number of prime factors (with multiplicity) shared by n and
 *          sigma(n): a(n) = bigomega(gcd(n, sigma(n))).
 * https://oeis.org/A341523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(gcd(n, sigma(n)))
 * Range:     u8 n <= 912, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9835 (2026-11-01)
 * Helpers:   bigomega, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t g = gcd((seq_t)n, sigma);
    return bigomega((idx_t)g);
}
