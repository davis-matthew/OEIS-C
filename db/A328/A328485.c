/*
 * A328485  Dirichlet g.f.: zeta(s)^2 * zeta(s-1) / zeta(2*s-1).
 * https://oeis.org/A328485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e||n} [ (p^(e+1)-p)/(p-1) + e + 1 ] (a(1) = 1)
 * Range:     u8 n <= 16, u16 n <= 786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt, prime_power_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"
#include "../utils/prime_power_val.h"

static uint64_t *prime_list = NULL;   /* cached list of primes */
static size_t prime_cnt   = 0;
static idx_t   prime_lim   = 0;

/* Ensure that prime_list contains all primes ≤ limit. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_lim) return;

    uint64_t *new_list = NULL;
    size_t   new_cnt   = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_lim  = limit;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, behaviour for 0 irrelevant */
    if (n == 1) return 1;   /* multiplicative neutral element */

    /* Prime sieve needed up to √n for factorisation. */
    idx_t limit = int_sqrt(n);
    if (limit < 2) limit = 2;
    ensure_primes(limit);

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_power_val(p, e);
        }
    }

    /* Remaining prime factor greater than √original_n. */
    if (temp > 1) {
        result *= prime_power_val(temp, 1);
    }

    return result;
}
