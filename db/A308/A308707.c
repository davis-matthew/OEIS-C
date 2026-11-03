/*
 * A308707  a(n) = gcd(n, phi(n) + sigma(n)), where phi is A000010 and sigma
 *          is A000203.
 * https://oeis.org/A308707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, phi(n) + sigma(n))
 * Range:     u8 n <= 89, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

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
    /* a(0) not defined in OEIS; return 0 to be safe */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;           /* cast n to seq_t */
    seq_t phi_n = totient(n);          /* Euler's totient */
    seq_t sigma_n = sigma_of(n);   /* sum of divisors */
    seq_t sum = phi_n + sigma_n;   /* phi(n) + sigma(n) */

    return gcd(sn, sum);           /* gcd of n and the sum */
}
