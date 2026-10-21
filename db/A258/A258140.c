/*
 * A258140  Number of ways to write 6*n + 2 as p^2 + q with p and q both
 *          prime.
 * https://oeis.org/A258140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 6*n+2 as p^2 + q with p and q
 *            primes. Search over primes p <= sqrt(6*n+2), count each q =
 *            6*n+2 - p^2 that is prime
 * Range:     u8 n <= 10, u16 n <= 1822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 6*n + 2 as seq_t to avoid overflow of intermediate 64‑bit multiplication */
    seq_t num = (seq_t)n * 6 + 2;

    /* sqrt_num = floor(sqrt(num)) */
    seq_t sqrt_num = floor_sqrt(num);
    idx_t limit = (idx_t)sqrt_num;

    /* static cache of primes up to the largest limit requested */
    static idx_t primes_limit = 0;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (limit > primes_limit) {
        if (primes) {
            free(primes);
            primes = NULL;
        }
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }

    seq_t count = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t p_sq = (seq_t)p * (seq_t)p;
        if (p_sq > num) {
            break;
        }
        seq_t q = num - p_sq;
        if (is_prime_big(q)) {
            ++count;
        }
    }
    return count;
}
