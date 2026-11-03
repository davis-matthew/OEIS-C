/*
 * A400415  a(n) = (2*n-sigma(n))/gcd(tau(n), 2*n-sigma(n)), tau = A000005,
 *          sigma = A000203.
 * https://oeis.org/A400415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, new
 * Formula:   a(n) = (2*n - sigma(n)) / gcd(tau(n), 2*n - sigma(n))
 * Range:     i8 n <= 167, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9217 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

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

/* Compute the n-th term of OEIS sequence A400415.
   idx_t is an unsigned 64‑bit integer.  seq_t may be any signed integer type
   (including arbitrary‑precision). */
seq_t a(idx_t n)
{
    seq_t two_n = ((seq_t)n) * 2;          /* 2*n as seq_t */
    seq_t sigma = sigma_of(n);            /* sigma(n) */
    seq_t diff  = two_n - sigma;          /* 2*n - sigma(n), may be negative */

    /* |diff| for gcd */
    seq_t absdiff = diff < 0 ? -diff : diff;
    /* tau(n) = number of divisors */
    seq_t tau = (seq_t)num_divisors(n);

    seq_t g = gcd(absdiff, tau);          /* gcd(|diff|, tau) */
    return diff / g;                      /* signed division preserves sign */
}
