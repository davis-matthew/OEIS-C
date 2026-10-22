/*
 * A381132  If n = (p_1^e_1)*(p_2^e_2)*(p_3^e_3)*... and
 *          min(p_1^e_1,p_2^e_2,...) = p_k^e_k then a(n) = pi(p_k), a(1) = 0.
 * https://oeis.org/A381132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(p_k), where p_k^e_k is the smallest prime power factor
 *            of n (p_k is the base prime); a(1) = 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/compute_pi.h"

/* cache of primes up to the current limit */
static uint64_t *g_primes = NULL;
static size_t   g_primes_cnt = 0;
static idx_t    g_primes_limit = 0;

/* extend or rebuild the prime list up to at least `limit`. */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_primes_limit) return;
    free(g_primes);
    g_primes = NULL;
    g_primes_cnt = 0;
    g_primes_limit = 0;
    build_primes_upto(limit, &g_primes, &g_primes_cnt);
    g_primes_limit = limit;
}

/* return the prime base of the smallest prime power dividing n. */
static uint64_t min_prime_base(idx_t n)
{
    if (n == 1) return 0;              /* a(1) is defined as 0 */

    /* initial sentinel value larger than any possible prime power. */
    seq_t best_power = (seq_t)n + 1;
    uint64_t best_prime = 0;
    int found_factor = 0;

    idx_t sqrt_n = _floor_isqrt_u64(n);
    if (sqrt_n < 2) sqrt_n = 2;
    ensure_primes(sqrt_n);

    for (size_t i = 0; i < g_primes_cnt; ++i) {
        uint64_t p = g_primes[i];
        if (p > sqrt_n) break;
        if (n % p == 0) {
            found_factor = 1;
            seq_t power = p;
            idx_t temp = n / p;
            while (temp % p == 0) {
                temp /= p;
                power *= p;        /* p^k */
            }
            if (power < best_power) {
                best_power = power;
                best_prime = p;
            }
        }
    }

    if (!found_factor) {               /* n is prime */
        best_prime = n;
    }

    return best_prime;
}

seq_t a(idx_t n)
{
    uint64_t p = min_prime_base(n);
    return compute_pi((idx_t)p);
}
