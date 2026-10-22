/*
 * A364937  a(n) is the least prime p such that p - prime(n) and p + prime(n)
 *          are triprimes.
 * https://oeis.org/A364937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least prime p such that p – prime(n) and p +
 *            prime(n) * are triprimes (products of exactly three primes
 *            counting multiplicity)
 * Range:     u8 n <= 38, u16 n <= 6301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, is_triprime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_triprime.h"

/* Forward declaration of the triprime tester */
static bool is_triprime(uint64_t m, const uint64_t *primes, size_t primes_cnt);

/* Public entry point required by the build system */
seq_t a(idx_t n)
{
    /* lazy static construction of a prime table for factorisation */
    static bool init = false;
    static uint64_t *factor_primes = NULL;
    static size_t factor_cnt = 0;

    if (!init) {
        /* 200 000 is comfortably larger than any prime needed for n ≤ 10 000 */
        const idx_t FP_LIMIT = 200000;
        build_primes_upto(FP_LIMIT, &factor_primes, &factor_cnt);
        init = true;
    }

    /* nth prime (OEIS indexing starts at 1) */
    uint64_t q = (uint64_t)get_prime(n);

    /* search for the smallest prime p > q satisfying the conditions */
    uint64_t p = next_prime(q);
    while (true) {
        uint64_t m1 = p - q;
        uint64_t m2 = p + q;

        if (is_triprime(m1, factor_primes, factor_cnt) &&
            is_triprime(m2, factor_primes, factor_cnt))
            return (seq_t)p;

        p = next_prime(p);
    }
}
