/*
 * A328456  LCM of the prime indices of 2n + 1, all minus 1; a(0) = 0.
 * https://oeis.org/A328456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM of the prime indices of 2*n + 1, all minus 1; a(0) =
 *            0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   build_primes_upto, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"
#include "../utils/build_primes_upto.h"

/* LCM of two seq_t values, treating a zero as a neutral element. */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == 0) return b;
    if (b == 0) return a;
    seq_t g = seq_gcd(a, b);
    return (a / g) * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute t = 2*n + 1.  Use uint64_t to avoid overflow. */
    uint64_t t = 2 * (uint64_t)n + 1;

    /* Static cache of the prime list, expanded as needed. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t   primes_limit = 0;

    if (t > primes_limit) {
        uint64_t *newprimes = NULL;
        size_t newcnt = 0;
        build_primes_upto(t, &newprimes, &newcnt);
        if (primes) free(primes);
        primes      = newprimes;
        primes_cnt  = newcnt;
        primes_limit = t;
    }

    seq_t lcm_val = 0;          /* cumulative LCM */
    uint64_t val  = t;          /* remaining factor to process */

    /* Trivial trial division using the cached primes. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > val) break;
        if (val % p == 0) {
            seq_t pos = (seq_t)i;              /* (prime index – 1) */
            lcm_val = lcm_seq(lcm_val, pos);
            while (val % p == 0) val /= p;     /* remove all powers of p */
        }
    }

    /* If a prime remains > sqrt(t), it is the last factor. */
    if (val > 1) {
        size_t idx = 0;
        for (; idx < primes_cnt; ++idx)
            if (primes[idx] == val) break;
        seq_t pos = (seq_t)idx;
        lcm_val = lcm_seq(lcm_val, pos);
    }

    return lcm_val;
}
