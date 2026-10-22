/*
 * A386062  Primes having only {0, 3, 5, 7} as digits.
 * https://oeis.org/A386062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 106, u32 n <= 47, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the primes consisting only of the digits
   0, 3, 5, 7 by length‑by‑length generation. For each new requested
   index n, all missing primes are generated and cached.  The helper
   function `append_candidate` is used for dynamic array growth. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Cache of all found primes in the required order */
static seq_t *prime_cache = NULL;    /* dynamic array of primes */
static size_t prime_cache_len = 0;   /* number of stored primes      */
static size_t prime_cache_cap = 0;   /* current capacity             */
static size_t max_requested   = 0;    /* the maximum n seen so far    */
static size_t next_len        = 1;    /* length of the next batch     */

/* Allowed digits in order of increasing value */
static const seq_t DIGITS[4] = {0, 3, 5, 7};

/* Recursive generation of all numbers of a given length `len`.
   Whenever a complete number of length `len` is formed, it is tested
   for primality and appended to the cache if prime.  No early
   termination is performed – all candidates of the given length
   are examined – because future requests might need any of them. */
static void rec_gen(size_t pos, size_t len, seq_t num)
{
    if (pos == len) {
        if (is_prime_big(num))
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap, num);
        return;
    }
    size_t start = (pos == 0) ? 1 : 0;          /* first digit cannot be zero */
    for (size_t d = start; d < 4; ++d) {
        rec_gen(pos + 1, len, num * 10 + DIGITS[d]);
    }
}

/* Generate primes of the next length and append them to the cache. */
static void generate_len(size_t len)
{
    rec_gen(0, len, (seq_t)0);
}

/* Return the n-th prime having only the digits {0,3,5,7}.  The function
   caches all previously found primes in a static array so that repeated
   calls with arbitrary order of n remain fast. */
seq_t a(idx_t n)
{
    size_t requested = (size_t)n;           /* n is at least 1 */

    /* Generate additional primes until we have at least `requested` of them. */
    if (requested > max_requested) {
        max_requested = requested;
        while (prime_cache_len < max_requested) {
            generate_len(next_len++);
        }
    }
    return prime_cache[requested - 1];
}
