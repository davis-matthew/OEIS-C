/*
 * A324528  a(n) = lcm(tau(n), pod(n)) where tau(k) = the number of divisors
 *          of k (A000005) and pod(n) = the product of divisors of k
 *          (A007955).
 * https://oeis.org/A324528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(tau(n), pod(n)) tau(n) = number of divisors of n,
 *            pod(n) = product of divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Validated: standard, pass, 63 terms through n = 992 (2026-11-01)
 * Helpers:   gcd, ipow, ipow_i32, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"
#include "../utils/num_divisors.h"

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
    /* tau(n) via built‑in divisor count */
    seq_t tau = (seq_t)num_divisors((uint64_t)n);

    seq_t pod;
    if ((tau & 1) == 0) {
        /* tau even: pod = n^(tau/2) */
        pod = ipow((seq_t)n, tau / 2);
    } else {
        /* tau odd: pod = sqrt(n) * n^((tau-1)/2) */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        pod = ipow((seq_t)n, (tau - 1) / 2);
        pod = pod * sqrt_n;
    }

    /* lcm(tau, pod) = tau / gcd(tau, pod) * pod */
    seq_t g = gcd(tau, pod);
    return (tau / g) * pod;
}
