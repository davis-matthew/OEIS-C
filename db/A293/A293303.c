/*
 * A293303  Exponential convolution of the exponential Mobius function and the
 *          natural numbers.
 * https://oeis.org/A293303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mu_exp_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A293303 : Exponential convolution of the exponential Möbius function and the natural numbers.
 *  Formula: a(n) = Π_{p^e‖n} Σ_{d|e} μ_exp(e/d)·p^d
 *  where μ_exp(m) = 0 if m has a squared prime factor; μ_exp(m) = (-1)^k if m is square‑free with k distinct primes.
 *  The function is multiplicative and computes a(n) by factorising n and evaluating the local prime‑power contributions.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mu_exp_uint.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Compute a(p^e) for a single prime power p^e. */
static seq_t prime_power_value(uint64_t p, idx_t exp)
{
    if (exp == 0) return 1;  /* should not happen */
    seq_t pow = 1;
    long long sum = 0;       /* signed intermediate */
    for (idx_t d = 1; d <= exp; ++d) {
        pow *= (seq_t)p;                 /* p^d */
        if (exp % d == 0) {
            int mu = mu_exp_uint(exp / d);
            if (mu != 0) sum += (long long)mu * (long long)pow;
        }
    }
    return (seq_t)sum;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused because offset is 1 */
    if (n == 1) return 1;   /* a(1) = 1 */

    idx_t remaining = n;
    seq_t result = 1;

    /* Factorise remaining by trial division up to sqrt(remaining) */
    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            idx_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            seq_t ap = prime_power_value(p, exp);
            result *= ap;
        }
    }

    /* If any prime factor > sqrt(n) remains */
    if (remaining > 1) {
        seq_t ap = prime_power_value(remaining, 1);
        result *= ap;
    }

    return result;
}
