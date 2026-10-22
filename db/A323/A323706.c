/*
 * A323706  a(n) = numerator of Sum_{d|n} tau(d)/pod(d) where tau(k) = the
 *          number of the divisors of k (A000005) and pod(k) = the product of
 *          the divisors of k (A007955).
 * https://oeis.org/A323706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n} tau(d)/pod(d) where tau is the
 *            divisor count and pod the product of divisors
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow, factor_u64, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

/* add the fraction tau/pod to the current fraction num/den */
static void add_fraction(seq_t tau, seq_t pod, seq_t *num, seq_t *den)
{
    seq_t g1 = gcd(*den, pod);
    seq_t den_div_g = *den / g1;
    seq_t pod_div_g = pod / g1;

    seq_t new_num = (*num) * pod_div_g + tau * den_div_g;
    seq_t new_den = (*den) * pod_div_g;

    seq_t g2 = gcd(new_num, new_den);
    *num = new_num / g2;
    *den = new_den / g2;
}

/* recursively enumerate all divisors of n
   cur_tau: tau of the current divisor
   cur_d  : value of the current divisor
*/
static void dfs_divisors(size_t idx,
                         uint64_t *primes, uint64_t *exps, size_t nPr,
                         seq_t cur_tau, seq_t cur_d,
                         seq_t *num, seq_t *den)
{
    if (idx == nPr) {
        /* compute pod(d) = d^{tau/2}  (perfect square handled below) */
        seq_t pod;
        if ((cur_tau & 1) == 0) { /* even tau */
            pod = big_pow(cur_d, cur_tau / 2);
        } else { /* odd tau -> d is a perfect square */
            seq_t sqrt_d = floor_sqrt(cur_d);
            seq_t exp = (cur_tau - 1) / 2;
            pod = big_pow(cur_d, exp);
            pod = pod * sqrt_d;
        }
        add_fraction(cur_tau, pod, num, den);
        return;
    }

    uint64_t p = primes[idx];
    uint64_t emax = exps[idx];
    /* precompute powers of p to avoid repeated multiplication */
    seq_t p_pow[21];  /* exps[idx] <= 60 for n<=1e18 so 20 is safe */
    p_pow[0] = (seq_t)1;
    for (uint64_t e = 1; e <= emax; ++e)
        p_pow[e] = p_pow[e - 1] * (seq_t)p;

    for (uint64_t e = 0; e <= emax; ++e) {
        seq_t new_d = cur_d * p_pow[e];
        seq_t new_tau = cur_tau * (seq_t)(e + 1);
        dfs_divisors(idx + 1, primes, exps, nPr,
                     new_tau, new_d, num, den);
    }
}

/* simple cache to avoid repeated work for up to MAX_CACHE */
#define MAX_CACHE 100000

static seq_t cache[MAX_CACHE + 1];
static bool cached[MAX_CACHE + 1];

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if (n <= MAX_CACHE && cached[n]) {
        return cache[n];
    }

    /* factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t num = (seq_t)0;   /* numerator of the sum */
    seq_t den = (seq_t)1;   /* denominator of the sum */

    dfs_divisors(0, primes, exps, cnt, (seq_t)1, (seq_t)1, &num, &den);

    if (n <= MAX_CACHE) {
        cached[n] = true;
        cache[n] = num;
    }

    return num;
}
