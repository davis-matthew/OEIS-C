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
 * Helpers:   big_pow, divisor_count_u64, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/floor_sqrt.h"
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* tau(n) via built‑in divisor count */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    seq_t pod;
    if ((tau & 1) == 0) {
        /* tau even: pod = n^(tau/2) */
        pod = big_pow((seq_t)n, tau / 2);
    } else {
        /* tau odd: pod = sqrt(n) * n^((tau-1)/2) */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        pod = big_pow((seq_t)n, (tau - 1) / 2);
        pod = pod * sqrt_n;
    }

    /* lcm(tau, pod) = tau / gcd(tau, pod) * pod */
    seq_t g = gcd(tau, pod);
    return (tau / g) * pod;
}
