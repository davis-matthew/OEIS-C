/*
 * A334985  a(n) = lcm(n, tau(n), sigma(n), pod(n)) / gcd(n, tau(n), sigma(n),
 *          pod(n)) where tau(k) is the number of divisors of k (A000005),
 *          sigma(k) is the sum of divisors of k (A000203) and pod(k) is the
 *          product of divisors of k (A007955).
 * https://oeis.org/A334985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, tau(n), sigma(n), pod(n)) / gcd(n, tau(n),
 *            sigma(n), pod(n))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

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
    if (n == 0) return (seq_t)0; /* undefined for 0, but harmless */

    /* factorisation of n: primes[], exponents[], pcnt */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* tau(n) = product (e_i + 1) */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* sigma(n) = product (p_i^(e_i+1)-1)/(p_i-1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t term = ipow_idx(p, (idx_t)(exps[i] + 1));
        term = (term - (seq_t)1) / (p - (seq_t)1);
        sigma *= term;
    }

    /* pod(n) = product of divisors = n^(tau/2) if tau even,
       otherwise n^(tau/2)*sqrt(n) */
    seq_t pod = ipow_idx((seq_t)n, (idx_t)(tau / (seq_t)2));
    if ((tau & (seq_t)1) != (seq_t)0) {          /* tau odd */
        pod *= floor_sqrt((seq_t)n);
    }

    /* gcd of all four numbers */
    seq_t g = gcd((seq_t)n, tau);
    g = gcd(g, sigma);
    g = gcd(g, pod);

    /* lcm of all four numbers */
    seq_t l = (seq_t)n;
    l = l / gcd(l, tau) * tau;
    l = l / gcd(l, sigma) * sigma;
    l = l / gcd(l, pod) * pod;

    return l / g;
}
