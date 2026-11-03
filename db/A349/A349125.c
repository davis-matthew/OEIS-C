/*
 * A349125  Dirichlet inverse of A064989, where A064989 is multiplicative with
 *          a(2^e) = 1 and a(p^e) = prevprime(p)^e for odd primes p.
 * https://oeis.org/A349125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative with a(p^e)=0 if e>1, -1 if p=2,
 *            -prevprime(p) otherwise
 * Range:     i8 n <= 136, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9414 (2026-11-02)
 * Helpers:   build_primes_upto, is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

static const uint64_t *prime_list = NULL;   /* primes ≤ current_limit */
static size_t prime_count = 0;
static idx_t current_limit = 0;

static void ensure_prime_list(idx_t limit)
{
    if (limit <= current_limit)
        return;
    if (prime_list)
        free((void *)prime_list);
    build_primes_upto(limit, (uint64_t **)&prime_list, &prime_count);
    current_limit = limit;
}

static seq_t factor_from_prime(uint64_t p)
{
    if (p == 2ULL)
        return (seq_t)-1;                    /* a(2) = -1 */
    return (seq_t)- (seq_t)prev_prime(p);    /* a(p) = -prevprime(p) for odd p */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t rem = (uint64_t)n;
    idx_t sqrt_n = isqrt(rem);   /* correct function name */
    ensure_prime_list(sqrt_n);

    seq_t result = (seq_t)1;
    bool squarefree = true;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > sqrt_n)
            break;
        if (rem % p == 0ULL) {
            int exp = 0;
            while (rem % p == 0ULL) {
                rem /= p;
                ++exp;
                if (exp > 1) {
                    squarefree = false;
                    break;
                }
            }
            if (!squarefree)
                break;
            result *= factor_from_prime(p);
        }
    }

    if (!squarefree)
        return (seq_t)0;

    if (rem > 1ULL)
        result *= factor_from_prime(rem);

    return result;
}
