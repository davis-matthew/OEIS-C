/*
 * A334731  a(n) = Product_{d|n} gcd(sigma(d), pod(d)) where sigma(k) is the
 *          sum of divisors of k (A000203) and pod(k) is the product of
 *          divisors of k (A007955).
 * https://oeis.org/A334731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} gcd(σ(d), pod(d))
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 119, u64 n <= 419,
 *            u128 n <= 1679, big any n
 * Validated: standard, pass, 90 terms through n = 9729 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx, isqrt_seq, num_divisors, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ(k) is the sum of divisors (A000203). pod(k) is the product of divisors (A007955). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"
#include "../utils/isqrt_seq.h"
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

/* --------------------------------------------------------------------- */
/* Compute gcd(σ(d), pod(d)) without overflow for 64‑bit builds.         */
/* If seq_t is 64‑bit, we compute pod modulo σ(d); otherwise we use the
 * full value.                                                              */
/* --------------------------------------------------------------------- */
static seq_t gcd_sigma_pod(uint64_t d)
{
    /* sum of divisors */
    seq_t sigma = sigma_of(d);

    /* divisor count τ(d) */
    uint64_t tau = num_divisors(d);

    /* Compute pod(d) modulo sigma when seq_t is 64‑bit. */
    seq_t pod_mod;

    if ((tau & 1ULL) == 0) {                           /* τ even  */
        if (sizeof(seq_t) <= 8) {                     /* 64‑bit build */
            pod_mod = powmod((seq_t)d, tau / 2, sigma);
        } else {
            pod_mod = ipow_idx((seq_t)d, tau / 2);
        }
    } else {                                            /* τ odd  */
        uint64_t exp = (tau - 1ULL) / 2;
        if (sizeof(seq_t) <= 8) {                     /* 64‑bit build */
            seq_t part = powmod((seq_t)d, exp, sigma);
            seq_t sqrt_d = isqrt_seq((seq_t)d);
            pod_mod = (part * (sqrt_d % sigma)) % sigma;
        } else {
            seq_t part   = ipow_idx((seq_t)d, exp);
            seq_t sqrt_d = isqrt_seq((seq_t)d);
            pod_mod = part * sqrt_d;
        }
    }
    return gcd(sigma, pod_mod);
}

/* --------------------------------------------------------------------- */
/* Recursive product over all divisors of n.                               */
/* --------------------------------------------------------------------- */
static seq_t prod_rec(size_t pos, uint64_t cur,
                      uint64_t *pr, uint64_t *ex, size_t npr)
{
    if (pos == npr) {               /* cur is a divisor of n */
        return gcd_sigma_pod(cur);
    }

    seq_t prod = (seq_t)1;
    uint64_t cur_e = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        seq_t sub = prod_rec(pos + 1, cur_e, pr, ex, npr);
        prod *= sub;
        cur_e *= pr[pos];
    }
    return prod;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Factor n into prime bases and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product over all divisors. */
    return prod_rec(0, 1ULL, primes, exps, cnt);
}
