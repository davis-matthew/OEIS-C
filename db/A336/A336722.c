/*
 * A336722  a(n) = gcd(tau(n), sigma(n), pod(n)) where tau(k) is the number of
 *          divisors of k (A000005), sigma(k) is the sum of divisors of k
 *          (A000203) and pod(k) is the product of divisors of k (A007955).
 * https://oeis.org/A336722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(tau(n), sigma(n), pod(n))
 * Range:     u8 n <= 1824, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8111 (2026-11-01)
 * Helpers:   isqrt, num_divisors, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n)   = number of divisors of n                        */
/* sigma(n) = sum of divisors of n                           */
/* pod(n)   = product of divisors of n, i.e. n^{tau(n)/2}    */
/* (for perfect squares multiply by sqrt(n) once)           */

/* Include helpers from the utilities library */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isqrt.h"
#include "../utils/num_divisors.h"
#include "../utils/powmod.h"

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

/* Small gcd routine that works for all seq_t types */
static inline seq_t _gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* All numbers are ≥ 1 in the OEIS definition */
    uint64_t nn = (uint64_t)n;

    /* 1. τ(n) – number of divisors */
    uint64_t tau = num_divisors(nn);

    /* 2. σ(n) – sum of divisors */
    seq_t sigma = sigma_of(nn);

    /* 3. g = gcd( τ(n), σ(n) ) */
    seq_t g = _gcd_seq((seq_t)tau, sigma);
    if (g == (seq_t)1) return (seq_t)1;   /* early exit if coprime */

    /* 4. Compute pod(n) modulo g */
    /*    pod(n) = n^{tau/2} * (sqrt(n) if tau odd) */
    idx_t exp_half = tau / 2;            /* floor(tau/2) */
    seq_t pod_mod = powmod((seq_t)nn, exp_half, g);

    if (tau & 1U) {                      /* if tau is odd, multiply by sqrt(n) once */
        uint64_t sqrt_n = isqrt(nn);
        pod_mod = (pod_mod * (seq_t)sqrt_n) % g;
    }

    /* 5. Final result: gcd( g, pod(n) ) = gcd( g, pod_mod ) */
    return _gcd_seq(g, pod_mod);
}
