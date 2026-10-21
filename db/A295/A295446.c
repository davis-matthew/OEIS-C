/*
 * A295446  a(n) = (10*n)!*(6*n)!/((9*n)!*(5*n)!*(2*n)!).
 * https://oeis.org/A295446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)! * (6*n)! / ((9*n)! * (5*n)! * (2*n)!) =
 *            binomial(10*n,n) * binomial(6*n,n) / binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   build_primes, factorial_prime_exp, pow_seq_large, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/pow_seq_large.h"
#include "../utils/build_primes.h"
#include "../utils/factorial_prime_exp.h"

static void ensure_primes(idx_t limit, idx_t **primes, size_t *cnt)
{
    /* Sieve primes up to limit and store in *primes.  Re‑allocate
       if the limit has increased.  The caller must free(*primes).   */
    static idx_t *cache = NULL;
    static size_t cache_cnt = 0;
    static idx_t cache_limit = 0;

    if (limit <= cache_limit) {
        *primes = cache;
        *cnt = cache_cnt;
        return;
    }

    if (cache) free(cache);
    size_t tmp_cnt;
    build_primes(limit, &cache, &tmp_cnt);
    cache_limit = limit;
    cache_cnt = tmp_cnt;

    *primes = cache;
    *cnt = cache_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t ten_n  = 10 * n;
    idx_t nine_n = 9 * n;
    idx_t six_n  = 6 * n;
    idx_t five_n = 5 * n;
    idx_t two_n  = 2 * n;

    /* Get all primes up to 10*n once, reuse for later calls */
    static idx_t *prime_list = NULL;
    static size_t prime_cnt  = 0;
    static idx_t max_prime_cache = 0;

    if (ten_n > max_prime_cache) {
        if (prime_list) free(prime_list);
        build_primes(ten_n, &prime_list, &prime_cnt);
        max_prime_cache = ten_n;
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        idx_t exp = factorial_prime_exp(ten_n, p)   +
                    factorial_prime_exp(six_n, p)   -
                    factorial_prime_exp(nine_n, p)   -
                    factorial_prime_exp(five_n, p)   -
                    factorial_prime_exp(two_n, p);
        if (exp > 0) {
            seq_t powp = pow_seq_large((seq_t)p, (seq_t)exp);
            result = seq_mul(result, powp);
        }
    }
    return result;
}
