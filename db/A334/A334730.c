/*
 * A334730  a(n) = Product_{d|n} gcd(tau(d), pod(d)) where tau(k) is the
 *          number of divisors of k (A000005) and pod(k) is the product of
 *          divisors of k (A007955).
 * https://oeis.org/A334730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{d|n} gcd(tau(d), pod(d))
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 215, u64 n <= 719,
 *            u128 n <= 4319, big any n
 * Helpers:   divisor_count_u64, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses divisor enumeration, divisor count, and
 * modulo reduction to compute gcd(tau(d), pod(d)) efficiently. */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"

/* Integer square root (floor). */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = n;
    uint64_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* Modular exponentiation: base^exp mod mod (mod > 0). */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)((unsigned __int128)result * base % mod);
        base = (uint64_t)((unsigned __int128)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Recursive divisor enumeration.  For each divisor cur, multiply
 * *prod_ptr by gcd(tau(cur), pod(cur)). */
static void dfs_divisors(size_t idx, uint64_t cur, const uint64_t *primes,
                         const uint64_t *exps, size_t pcnt, seq_t *prod_ptr)
{
    if (idx == pcnt) {
        uint64_t d = cur;
        uint64_t tau = divisor_count_u64(d);
        uint64_t mod = tau;
        uint64_t pod_mod;

        if (mod == 0) { /* tau cannot be 0 */
            pod_mod = 0;
        } else if (tau & 1) {                /* tau odd: d is a perfect square */
            uint64_t s = isqrt_u64(d);
            uint64_t exp = (tau - 1) / 2;
            uint64_t powmod = mod_pow_u64(d, exp, mod);
            pod_mod = (s % mod) * powmod % mod;
        } else {                              /* tau even */
            uint64_t exp = tau / 2;
            pod_mod = mod_pow_u64(d, exp, mod);
        }

        *prod_ptr = (*prod_ptr) * gcd((seq_t)tau, (seq_t)pod_mod);
        return;
    }

    uint64_t cur_local = cur;
    uint64_t p = primes[idx];
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        dfs_divisors(idx + 1, cur_local, primes, exps, pcnt, prod_ptr);
        cur_local *= p;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 is assumed by the caller (offset 1) */
    uint64_t nn = (uint64_t)n;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);

    seq_t prod = 1;
    dfs_divisors(0, 1, primes, exps, pcnt, &prod);
    return prod;
}
