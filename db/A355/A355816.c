/*
 * A355816  a(n) = 1 if n and sigma(n) are relatively prime, and n has at
 *          least two distinct prime factors, otherwise 0.
 * https://oeis.org/A355816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n has at least two distinct prime factors and
 *            gcd(n,σ(n)) = 1; otherwise 0
 * Range:     u8 n <= 2048, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9296 (2026-11-01)
 * Helpers:   gcd, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/omega.h"

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
    if (n <= 1)
        return 0;                         /* undefined for n < 1, defined as 0 */

    /* n must have at least two distinct prime factors */
    if (omega((uint64_t)n) < 2)
        return 0;

    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* n and σ(n) must be coprime */
    return (gcd((seq_t)n, sigma) == 1) ? 1 : 0;
}
