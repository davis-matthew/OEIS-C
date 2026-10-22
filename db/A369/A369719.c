/*
 * A369719  The number of divisors of the smallest cubefull number that is a
 *          multiple of n.
 * https://oeis.org/A369719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = d( A356193(n) ), where A356193 gives the smallest
 *            cubefull multiple of n. The divisor count of that multiple is
 *            product over prime factors p^e of n of 4 if e <= 2 e+1 if e >= 3
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Handle the offset 1: for n = 1 the answer is 1. */
    if (n < 2)
        return (seq_t)1;

    /* Static cache for prime tables. */
    static uint64_t *prime_table = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    /* Ensure primes up to sqrt(n). */
    idx_t limit = floor_isqrt_u64(n) + 1;
    if (limit > prime_limit) {
        if (prime_table)
            free(prime_table);
        build_primes_upto(limit, &prime_table, &prime_count);
        prime_limit = limit;
    }

    uint64_t cur = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factor n using the cached primes. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_table[i];
        if (p > cur / p)   /* p*p > cur, stop early */
            break;
        if (cur % p == 0) {
            int exp = 0;
            while (cur % p == 0) {
                cur /= p;
                ++exp;
            }
            result *= (seq_t)((exp >= 3) ? (exp + 1) : 4);
        }
    }

    /* Remaining prime factor > sqrt(n).  Its exponent is 1. */
    if (cur > 1)
        result *= (seq_t)4;

    return result;
}
