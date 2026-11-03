/*
 * A291934  Largest k achievable by the iteration k->(sigma(k)+phi(k))/2
 *          started at k=n, with termination when sigma(k) becomes odd.
 * https://oeis.org/A291934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 152, u32 n <= 269, u64 n <= 269,
 *            u128 n <= 269, big any n
 * Validated: standard, pass, 83 terms through n = 259 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Starting with k = n, iterate k = (sigma(k)+phi(k))/2 until
   sigma(k) is odd or the value stabilises (k_next == k).  Return that
   final k, which is the largest value reachable under the given rule.
   sigma(k) is the sum of divisors of k, phi(k) is Euler's totient.  */

/* The helper functions sigma_of and phi are provided in the build
   utilities. */
#include <stdint.h>
#include <stdlib.h>
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
    seq_t k = (seq_t)n;          /* current value of the iteration   */

    while (1)
    {
        /* Convert to idx_t for the helper functions (assumes k fits in uint64). */
        idx_t k64 = (idx_t)k;

        seq_t sigma_k = sigma_of(k64);   /* σ(k) – sum of divisors          */
        if ((sigma_k & (seq_t)1) != 0)   /* σ(k) is odd → terminate        */
            return k;

        seq_t phi_k   = totient(k64);        /* φ(k) – Euler's totient          */
        seq_t k_next  = (sigma_k + phi_k) >> 1;   /* (σ+φ)/2 */

        if (k_next == k)                 /* fixed point – terminate         */
            return k;

        k = k_next;                      /* continue to next iteration      */
    }
}
