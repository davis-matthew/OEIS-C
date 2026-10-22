/*
 * A382872  For n >= 1, a(n) is the number of divisors of Pillai's
 *          arithmetical function: Sum_{k=1..n} gcd(k, n) (A018804).
 * https://oeis.org/A382872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d( sum_{k=1..n} gcd(k , n) ) where d(x) is the number of
 *            divisors of x. The sum is evaluated by the identity S(n) =
 *            Σ_{d|n} d · φ(n/d) (φ is Euler's totient), so S(n) can be
 *            computed from the prime factorization of n. The divisor count of
 *            S(n) is then obtained by trial division with pre‑co...
 * Range:     u8 n <= 19, u16 n <= 2698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Forward declaration of the recursive divisor enumeration */
static void rec_divisors(size_t idx, seq_t d, seq_t phi_q,
                         const uint32_t *pr, const uint32_t *ex,
                         size_t cnt, seq_t *sum);

/* Global cache of small primes up to 2 million (sufficient for all tests) */
static uint64_t *g_primes64 = NULL;
static size_t g_prime_cnt  = 0;

seq_t a(idx_t n)
{
    /* initialise small prime sieve once */
    if (g_primes64 == NULL) {
        build_primes_upto((idx_t)2000000, &g_primes64, &g_prime_cnt);
    }

    /* factorise n */
    uint64_t n64 = (uint64_t)n;
    uint64_t primes_n[32];
    uint64_t exps_n[32];
    size_t  cnt_n = 0;
    factor_u64(n64, primes_n, exps_n, &cnt_n);

    /* convert to uint32_t arrays for convenience */
    uint32_t pr[32], ex[32];
    for (size_t i = 0; i < cnt_n; ++i) {
        pr[i] = (uint32_t)primes_n[i];
        ex[i] = (uint32_t)exps_n[i];
    }

    /* compute S(n) = sum_{d|n} d * φ(n/d) */
    seq_t sum_val = 0;
    rec_divisors(0, (seq_t)1, (seq_t)1, pr, ex, cnt_n, &sum_val);

    /* obtain divisor count of sum_val */
    seq_t remaining = sum_val;
    seq_t result = (seq_t)1;

    seq_t sqrt_val = floor_sqrt(remaining);
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint32_t p = (uint32_t)g_primes64[i];
        seq_t psq = (seq_t)p;
        if (psq > sqrt_val) break;

        uint32_t exp_cnt = 0;
        while (remaining % psq == (seq_t)0) {
            remaining /= psq;
            ++exp_cnt;
        }
        if (exp_cnt) {
            result *= (seq_t)(exp_cnt + 1);
        }
    }
    if (remaining > (seq_t)1) {
        result *= (seq_t)2;
    }

    return result;
}

/* Recursive enumeration of all divisors of n from its factorisation,
   accumulating Σ d · φ(n/d).  The accumulation is done in *sum. */
static void rec_divisors(size_t idx, seq_t d, seq_t phi_q,
                         const uint32_t *pr, const uint32_t *ex,
                         size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        *sum += d * phi_q;
        return;
    }

    uint32_t p   = pr[idx];
    uint32_t exp = ex[idx];

    /* pre‑compute powers of p up to exp */
    seq_t pow_p[64];
    pow_p[0] = (seq_t)1;
    for (size_t k = 1; k <= exp; ++k) {
        pow_p[k] = pow_p[k - 1] * (seq_t)p;
    }

    for (uint32_t e = 0; e <= exp; ++e) {
        seq_t d_next = d * pow_p[e];
        seq_t phi_next;
        if (exp - e == 0) {
            phi_next = phi_q;                          /* φ(1) = 1   */
        } else {
            seq_t pow_rem = pow_p[exp - e - 1];         /* p^(e_rem-1) */
            phi_next = phi_q * ((seq_t)p - (seq_t)1) * pow_rem;
        }
        rec_divisors(idx + 1, d_next, phi_next, pr, ex, cnt, sum);
    }
}
