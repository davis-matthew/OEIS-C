/*
 * A376032  Least prime p that does not divide n such that 2^phi(n) == 1 (mod
 *          p), or 0 if no such prime exists.
 * https://oeis.org/A376032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime p not dividing n with 2^φ(n) ≡ 1 (mod p), or
 *            0 if none
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9737 (2026-11-01)
 * Helpers:   build_primes_upto, powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;
static idx_t     prime_limit = 0;

static void ensure_primes(idx_t min_limit)
{
    if (prime_list && prime_limit >= min_limit)
        return;
    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(min_limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list  = new_list;
    prime_count = new_cnt;
    prime_limit = min_limit;
}

seq_t a(idx_t n)
{
    if (n == 1 || n == 2 || n == 3 || n == 6)
        return (seq_t)0;

    seq_t phi = totient_seq(n);
    uint64_t phi_val = (uint64_t)phi;   /* phi(n) fits in 64 bits for the tested range */

    if (!prime_list)
        ensure_primes(100000);          /* initial prime table */

    for (;;)
    {
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint64_t p = prime_list[i];
            if ((n % p) == 0)
                continue;
            seq_t val = powmod((seq_t)2, phi_val, (seq_t)p);
            if (val == (seq_t)1)
                return (seq_t)p;
        }
        /* No suitable prime found in current table; enlarge it and retry. */
        prime_limit *= 2;
        ensure_primes(prime_limit);
    }
}
