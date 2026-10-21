/*
 * A257992  Number of even parts in the partition having Heinz number n.
 * https://oeis.org/A257992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even parts in the partition having Heinz number
 *            n
 * Range:     u8 n <= 9508, u16 n <= 9508, u32 n <= 9508, u64 n <= 9508,
 *            u128 n <= 9508, big any n
 * Helpers:   build_primes_upto, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The partition is determined by the prime factorisation of n. */
/* For each prime p dividing n, let j be the index of p in the sequence of primes. */
/* The partition contains part j repeated exponent times. */
/* a(n) counts all parts j that are even. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi.h"

/* Compute a(n) for 1 ≤ n. */
seq_t a(idx_t n)
{
    /* Static state: list of primes up to the largest n seen so far. */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t max_prime_limit = 0;

    /* Ensure we have all primes up to n. */
    if (n > max_prime_limit) {
        free(prime_list);
        build_primes_upto(n, &prime_list, &prime_count);
        max_prime_limit = n;
    }

    /* Edge case: a(1) = 0 (empty partition). */
    if (n == 1) return 0;

    uint64_t remaining = n;
    seq_t total = 0;

    /* Factorise using the pre‑computed prime list. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > remaining) break;

        int exp = 0;
        while (remaining % p == 0) {
            remaining /= p;
            ++exp;
        }
        if (exp && ((i + 1) % 2 == 0))  /* i+1 is the prime index. */
            total += exp;
    }

    /* If a prime factor remains, it is > sqrt(n) and occurs to the first power. */
    if (remaining > 1) {
        seq_t idx = prime_pi(remaining);  /* index of this prime (1‑based). */
        if ((idx % 2) == 0)               /* even index → even part. */
            total += 1;
    }

    return total;
}
