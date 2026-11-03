/*
 * A379111  a(n) = 1 if bigomega(sigma(n)) is equal to omega(n), otherwise 0.
 * https://oeis.org/A379111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (bigomega(sigma(n)) == omega(n) ? 1 : 0)
 * Range:     u8 n <= 7345, u16 n <= 102400, u32 n <= 102400, u64 n <= 102400,
 *            u128 n <= 102400, big any n
 * Validated: standard, pass, 121 terms through n = 8762 (2026-11-01)
 * Helpers:   bigomega, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega(n) = number of distinct prime factors of n */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
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
    /* omega(n): number of distinct prime factors (int) */
    int omega_n = omega((uint64_t)n);

    /* sigma(n): sum of divisors (seq_t) */
    seq_t sigma_n = sigma_of(n);

    /* bigomega(sigma(n)) (seq_t) */
    seq_t omega_sigma = bigomega((idx_t)sigma_n);

    /* Return 1 if the two values are equal, else 0 */
    return (omega_sigma == (seq_t)omega_n) ? (seq_t)1 : (seq_t)0;
}
