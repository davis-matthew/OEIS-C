/*
 * A390449  Heinz number of first sums of prime indices of n.
 * https://oeis.org/A390449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the Heinz number of the first sums of prime indices of n
 *            (definition as in OEIS A390449)
 * Range:     u8 n <= 71, u16 n <= 1457, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, compute_pi, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_pi.h"
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 2)            /* n = 1 => empty multiset => result 1 */
        return (seq_t)1;

    /* Build a prime list up to n the first time or whenever n is larger */
    static uint64_t *prime_tbl = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (prime_tbl == NULL || n > prime_limit) {
        if (prime_tbl) free(prime_tbl);
        build_primes_upto(n, &prime_tbl, &prime_cnt);
        prime_limit = n;
    }

    uint64_t m = (uint64_t)n;          /* mutable copy for factorisation */
    seq_t result = (seq_t)1;
    bool have_prev = false;
    uint64_t prev_idx = 0;

    for (size_t i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= m; ++i) {
        uint64_t p = prime_tbl[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t idx = compute_pi(p);            /* index of prime p */
            uint64_t idx64 = (uint64_t)idx;
            for (int k = 0; k < exp; ++k) {
                if (!have_prev) {
                    prev_idx = idx64;
                    have_prev = true;
                } else {
                    uint64_t sum = prev_idx + idx64;
                    seq_t prime_sum = get_prime(sum);
                    result = seq_mul(result, prime_sum);
                    prev_idx = idx64;
                }
            }
        }
    }

    /* Remaining prime factor greater than sqrt(n) */
    if (m > 1) {
        uint64_t p = m;
        seq_t idx = compute_pi(p);
        uint64_t idx64 = (uint64_t)idx;
        if (!have_prev) {
            prev_idx = idx64;
            have_prev = true;
        } else {
            uint64_t sum = prev_idx + idx64;
            seq_t prime_sum = get_prime(sum);
            result = seq_mul(result, prime_sum);
            prev_idx = idx64;
        }
    }

    return result;
}
