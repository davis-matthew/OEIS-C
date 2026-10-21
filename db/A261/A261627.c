/*
 * A261627  Number of primes p such that n-(p*n'-1) and n+(p*n'-1) are both
 *          prime, where n' is 1 or 2 according as n is odd or even.
 * https://oeis.org/A261627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A261627:
   a(n) = number of primes p such that n-(p*n'‑1) and n+(p*n'‑1) are both prime,
   where n' = 1 if n is odd, else 2. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

static uint64_t *prime_cache = NULL;
static size_t      prime_count = 0;
static idx_t       prime_limit = 0;

/* Ensure that all primes up to `limit` are available in `prime_cache`. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    free(prime_cache);
    build_primes_upto(limit, &prime_cache, &prime_count);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* n' = 1 if n odd, 2 if n even */
    uint8_t np = (n & 1) ? 1 : 2;

    /* For odd n we need primes < n.
       For even n we need primes < n/2.
       We ask for primes up to `limit`, which may include one extra prime
       that we will skip when the left value becomes ≤ 1. */
    idx_t limit = (np == 1) ? n : (n >> 1);
    if (limit < 1) limit = 1;          /* avoid zero limit for n == 1 */

    ensure_primes(limit);

    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_cache[i];

        /* left = n - (p*np - 1) = n - p*np + 1  */
        seq_t left = (seq_t)n - (seq_t)p * (seq_t)np + 1;
        if (left <= 1) break;           /* further p will only make left smaller */

        /* right = n + (p*np - 1) */
        seq_t right = (seq_t)n + (seq_t)p * (seq_t)np - 1;

        if (is_prime_big(left) && is_prime_big(right))
            ++count;
    }
    return count;
}
