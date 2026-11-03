/*
 * A324529  a(n) = lcm(sigma(n), pod(n)) where sigma(k) = the sum of divisors
 *          of k (A000203) and pod(n) = the product of divisors of k
 *          (A007955).
 * https://oeis.org/A324529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(σ(n), pod(n)) where σ(n) is the sum of divisors
 *            (A000203) and pod(n) is the product of divisors (A007955)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   gcd, ipow, ipow_i32, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* σ(n) */
    seq_t sigma = sigma_of(n);

    /* pod(n) = n^(d(n)/2) where d(n) is the number of divisors of n */
    uint64_t tau = num_divisors(n);   /* d(n) */
    seq_t pod;
    seq_t nn = (seq_t)n;
    if ((tau & 1ULL) == 0) {                 /* d(n) even */
        pod = ipow(nn, tau / 2);
    } else {                                 /* d(n) odd → n is a square */
        seq_t sqrt_n = floor_sqrt(nn);
        pod = ipow(nn, (tau - 1) / 2) * sqrt_n;
    }

    /* lcm(σ, pod) = σ * pod / gcd(σ, pod) */
    seq_t g = gcd(sigma, pod);
    seq_t l = sigma * pod;
    return l / g;
}
