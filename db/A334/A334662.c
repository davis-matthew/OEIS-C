/*
 * A334662  a(n) = Sum_{d|n} gcd(tau(d), pod(d)), where pod(k) is the product
 *          of the divisors of k (A007955).
 * https://oeis.org/A334662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} gcd(tau(d), pod(d)), where tau is the divisor
 *            count and pod is the product of the divisors. This
 *            implementation avoids overflow by computing pod modulo tau,
 *            using the property gcd(tau, pod) = gcd(tau, pod mod tau)
 * Range:     u8 n <= 63, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 94 terms through n = 8829 (2026-11-01)
 * Helpers:   gcd, ipow_i32, num_divisors, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/num_divisors.h"
#include "../utils/powmod.h"

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

static seq_t gcd_tau_pod(uint64_t d)
{
    /* tau(d) */
    seq_t tau = (seq_t)num_divisors(d);
    seq_t exp = tau / 2;          /* floor(tau / 2) */

    seq_t rem;
    if (tau & 1U) {               /* tau is odd – pod = d^exp * sqrt(d) */
        seq_t sqrt_d = floor_sqrt((seq_t)d);
        seq_t part   = powmod((seq_t)d, (uint64_t)exp, tau);
        seq_t sqrt_mod = sqrt_d % tau;
        rem = (part * sqrt_mod) % tau;
    } else {                      /* tau even – pod = d^exp */
        rem = powmod((seq_t)d, (uint64_t)exp, tau);
    }

    return gcd(tau, rem);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;  /* n ≥ 1 */
    seq_t total = 0;

    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            total += gcd_tau_pod(d);
            uint64_t other = N / d;
            if (other != d) {
                total += gcd_tau_pod(other);
            }
        }
    }
    return total;
}
