/*
 * A290753  Semiprimes beginning with 1 such that deleting the 1 leaves a
 *          prime.
 * https://oeis.org/A290753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer of the form 1p where p is prime, * 1p is
 *            a semiprime (product of two primes)
 * Range:     u8 n <= 10, u16 n <= 465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto, digits_of,
 *            floor_sqrt, is_prime_big, is_prime_small, next_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iterates over primes p, constructs N = 10^digits(p)+p,
 * checks whether N is a semiprime, and stores the qualifying
 * numbers in a cache. It works for any seq_t and idx_t supplied by the
 * build (8, 16, 32, 64 or 128 bit values, or arbitrary‑precision).
 * All required utilities are included from the ../utils directory. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/pow10.h"           /* seq_t pow10(idx_t e); */
#include "../utils/digits_of.h"       /* int digits_of(idx_t x); */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */
#include "../utils/is_prime_small.h"  /* bool is_prime_small(uint64_t n); */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t n); */
#include "../utils/floor_sqrt.h"      /* seq_t floor_sqrt(seq_t x); */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

static bool is_semiprime(seq_t N)
{
    /* Prime test */
    if (is_prime_big(N))
        return false;

    /* If N fits in uint64_t, use fast trial division */
    if ((seq_t)UINT64_MAX >= N) {
        uint64_t n64 = (uint64_t)N;
        uint64_t d = _smallest_prime_factor(n64);
        if (d == n64)     /* in principle unreachable because N is composite */
            return false;
        uint64_t other = n64 / d;
        return is_prime_small(other);
    }

    /* In general: trial division with small primes up to sqrt(N) */
    seq_t sqrtN = floor_sqrt(N);
    idx_t limit = (idx_t)sqrtN;            /* sqrtN fits into idx_t (see analysis) */

    /* Static storage for primes up to current limit */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0;
    static idx_t primes_limit = 0;

    if (limit > primes_limit) {
        /* Rebuild prime list up to new limit */
        free(primes);
        build_primes_upto(limit, &primes, &primes_len);
        primes_limit = limit;
    }

    /* Trial division */
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > N)
            break;
        if (N % (seq_t)p == 0) {
            seq_t other = N / (seq_t)p;
            return is_prime_big(other);
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure cache can hold at least n elements */
    while (cache_len < (size_t)n) {
        /* Next prime p */
        static uint64_t current_p = 1;
        current_p = next_prime(current_p + 1);

        int k = digits_of((idx_t)current_p);
        seq_t pow10k = pow10(k);
        seq_t candidate = pow10k + (seq_t)current_p;

        if (is_semiprime(candidate)) {
            /* Expand cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
    }
    return cache[n - 1];
}
