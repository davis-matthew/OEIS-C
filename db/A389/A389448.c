/*
 * A389448  Denominator of ratio sigma(n) / A057723(n), where A057723 is sum
 *          of positive divisors of n that are divisible by every prime that
 *          divides n and sigma is the sum of divisors function.
 * https://oeis.org/A389448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of σ(n) / (rad(n)·σ(n/rad(n)))
 * Range:     u8 n <= 143, u16 n <= 9958, u32 n <= 9958, u64 n <= 9958,
 *            u128 n <= 9958, big any n
 * Helpers:   gcd, rad, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/rad.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t S = sigma_of(n);                // σ(n)
    seq_t rad_n = rad(n);                 // rad(n)
    idx_t M = n / (idx_t)rad_n;           // n / rad(n)
    seq_t B = sigma_of(M);                // σ(n/rad(n))
    seq_t P = rad_n * B;                  // rad(n)·σ(n/rad(n))
    seq_t g = gcd(S, P);                  // gcd(σ(n), P)
    return P / g;                         // denominator after reduction
}
