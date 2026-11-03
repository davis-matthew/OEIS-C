/*
 * A389448  Denominator of ratio sigma(n) / A057723(n), where A057723 is sum
 *          of positive divisors of n that are divisible by every prime that
 *          divides n and sigma is the sum of divisors function.
 * https://oeis.org/A389448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of σ(n) / (rad(n)·σ(n/rad(n)))
 * Range:     u8 n <= 143, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9958 (2026-11-01)
 * Helpers:   gcd, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/radical.h"

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
    seq_t S = sigma_of(n);                // σ(n)
    seq_t rad_n = radical(n);                 // rad(n)
    idx_t M = n / (idx_t)rad_n;           // n / rad(n)
    seq_t B = sigma_of(M);                // σ(n/rad(n))
    seq_t P = rad_n * B;                  // rad(n)·σ(n/rad(n))
    seq_t g = gcd(S, P);                  // gcd(σ(n), P)
    return P / g;                         // denominator after reduction
}
