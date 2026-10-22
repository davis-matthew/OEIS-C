/*
 * A322312  a(n) = Product_{d|n, d+1 is prime} prime(1+A286561(n,d+1)), where
 *          A286561(n,k) gives the k-valuation of n (for k > 1).
 * https://oeis.org/A322312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d+1 is prime} prime(1 + A286561(n, d+1))
 * Range:     u8 n <= 35, u16 n <= 359, u32 n <= 7559, u64 n <= 9709,
 *            u128 n <= 9709, big any n
 * Helpers:   build_primes_upto, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Explicitly:  a(n) = ∏_{p prime, (p‑1) | n} prime(1 + v_p(n)) */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static primes cache – rebuild if n+1 exceeds current limit */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n + 1 > prime_limit) {
        if (prime_list) free(prime_list);
        build_primes_upto(n + 1, &prime_list, &prime_cnt);
        prime_limit = n + 1;
    }

    /* factorization of n */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64(n, factors, exps, &fcnt);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p   = prime_list[i];
        uint64_t step = p - 1;            /* d = p-1 */
        if (step == 0) continue;          /* p = 1 never occurs */
        if (n % step == 0) {              /* (p‑1) | n  */
            /* get v_p(n) from factorization (0 if not present) */
            uint64_t e = 0;
            for (size_t j = 0; j < fcnt; ++j) {
                if (factors[j] == p) { e = exps[j]; break; }
            }
            /* multiply by the (1+e)-th prime */
            idx_t idx = 1 + e;                    /* very small, fits in idx_t */
            seq_t prime_val = (seq_t)get_prime(idx);
            result *= prime_val;
        }
    }
    return result;
}
