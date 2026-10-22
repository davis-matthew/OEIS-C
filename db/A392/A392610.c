/*
 * A392610  Number of nonzero digits in the primorial base representation of n
 *          (A049345) such that their corresponding radix prime does not
 *          divide n.
 * https://oeis.org/A392610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of non‑zero digits in the primorial base *
 *            representation of n whose corresponding radix prime does not
 *            divide n
 * Range:     u8 n <= 9210, u16 n <= 9210, u32 n <= 9210, u64 n <= 9210,
 *            u128 n <= 9210, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Computes a(n) for the OEIS sequence A392610. */
seq_t a(idx_t n) {
    /* One‑time initialization of the prime list. */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    if (!initialized) {
        /* 100000 is more than enough for all practical n in the tests. */
        build_primes_upto(100000, &primes, &primes_cnt);
        initialized = true;
    }

    /* a(0) = 0 by definition. */
    if (n == 0)
        return 0;

    idx_t count = 0;
    uint64_t orig = n;       /* keep the original n for divisibility checks */
    size_t idx = 0;          /* index into the primes array */

    /* Greedy primorial base decomposition */
    while (n > 0 && idx < primes_cnt) {
        uint64_t p = primes[idx++];
        uint64_t digit = n % p;
        n /= p;
        if (digit != 0 && orig % p != 0)
            ++count;
    }

    return (seq_t)count;
}
