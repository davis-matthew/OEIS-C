/*
 * A316290  a(n) is the number of ways of writing prime(n) as the sum of a
 *          prime number and a number that has only prime factors 2 and/or 5.
 * https://oeis.org/A316290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of ways to write prime(n) as p + r where p is
 *            prime and r has only prime factors 2 and/or 5
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx(idx_t) */
#include "../utils/build_primes_upto.h"      /* build_primes_upto(idx_t, uint64_t **, size_t *) */

static uint64_t *prime_list   = NULL;   /* array of primes  */
static size_t     prime_cnt   = 0;       /* number of primes in prime_list   */
static idx_t      prime_limit = 0;       /* largest index of primes computed */

static void ensure_primes(idx_t n)
{
    /* The OEIS routine _prime_limit_for_idx(n) returns a sufficient
       upper bound for the n-th prime.  Compute the list up to that
       upper bound only when the current table is too small. */
    idx_t lim = _prime_limit_for_idx(n);
    if (lim <= prime_limit)
        return;
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    prime_cnt = 0;
    build_primes_upto(lim, &prime_list, &prime_cnt);
    prime_limit = lim;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* below the offset – undefined */

    ensure_primes(n);

    if (n > prime_cnt)
        return (seq_t)0;    /* requested prime not available */

    uint64_t p = prime_list[n - 1];          /* the n‑th prime */
    seq_t    count = 0;
    uint64_t max_r = p - 1;                  /* r must be < p */

    for (unsigned a = 0; a < 64; ++a) {
        uint64_t pow2 = 1ULL << a;
        if (pow2 > max_r)
            break;
        uint64_t pow5 = 1;
        while (true) {
            __uint128_t prod128 = (__uint128_t)pow2 * pow5;
            if (prod128 > max_r)
                break;
            uint64_t r = (uint64_t)prod128;
            uint64_t q = p - r;
            if (q >= 2 && is_prime((idx_t)q))
                count++;
            if (pow5 > max_r / 5)          /* would overflow next step */
                break;
            pow5 *= 5;
        }
    }
    return count;
}
