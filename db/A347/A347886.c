/*
 * A347886  Odd numbers k such that sigma(k^2) has an even number of prime
 *          factors when counted with multiplicity.
 * https://oeis.org/A347886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = the n-th odd k such that bigomega(sigma(k^2)) is even. */
/* Search method: iterate through odd k, compute sigma(k^2) using sigma_of,
   then count its prime factors with multiplicity using count_prime_factors.
   When the count is even, accept k. Return the nth accepted k. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"

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

seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t k = 1;              /* start with first odd integer */
    while (found < n) {
        idx_t kk = k * k;     /* k is odd, kk will fit in idx_t */
        seq_t s = sigma_of(kk);
        seq_t omega = bigomega_seq(s);
        if ((omega & 1) == 0) {   /* even number of prime factors */
            found++;
            if (found == n)
                return (seq_t)k;
        }
        k += 2;               /* move to next odd number */
    }
    return 0; /* unreachable – guard for compiler */
}
