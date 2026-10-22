/*
 * A329929  a(n) = lcm(tau(n), sigma(n), pod(n)) / gcd(tau(n), sigma(n),
 *          pod(n)) where tau(k) is the number of divisors of k (A000005),
 *          sigma(k) is the sum of divisors of k (A000203) and pod(k) is the
 *          product of divisors of k (A007955).
 * https://oeis.org/A329929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(tau(n), sigma(n), pod(n)) / gcd(tau(n), sigma(n),
 *            pod(n))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, factor_u64, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x) */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* ---- Factor n into primes ---- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ---- Compute tau(n) and sigma(n) ---- */
    seq_t tau = 1;
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        seq_t p = (seq_t)primes[i];

        tau *= (e + (seq_t)1);

        /* sigma term: (p^(e+1)-1)/(p-1) */
        seq_t numer = big_pow(p, e + (seq_t)1) - (seq_t)1;
        seq_t denom = p - (seq_t)1;
        seq_t term = numer / denom;
        sigma *= term;
    }

    /* ---- Compute pod(n) ---- */
    seq_t pod;
    seq_t pow_exp = tau >> 1;                 /* floor(tau/2) */
    pod = big_pow((seq_t)n, pow_exp);        /* n^floor(tau/2) */

    if (tau & (seq_t)1) {                    /* tau odd -> multiply by sqrt(n) */
        seq_t sqrt_n = floor_sqrt((seq_t)n);
        pod *= sqrt_n;
    }

    /* ---- GCD of all three ---- */
    seq_t g12 = gcd(tau, sigma);
    seq_t g123 = gcd(g12, pod);

    /* ---- LCM of all three, computed correctly ---- */
    /* lcm(sigma, pod) */
    seq_t g_sp = gcd(sigma, pod);
    seq_t l_sp = sigma / g_sp * pod;

    /* lcm(tau, l_sp) */
    seq_t g_tl = gcd(tau, l_sp);
    seq_t l_all = tau / g_tl * l_sp;

    /* ---- Result ---- */
    return l_all / g123;
}
