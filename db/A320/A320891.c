/*
 * A320891  Numbers with an even number of prime factors (counted with
 *          multiplicity) that cannot be factored into squarefree semiprimes.
 * https://oeis.org/A320891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A320891:  Numbers with an even number of prime factors (counted
 * with multiplicity) that cannot be factored into squarefree semiprimes.
 * Computation is performed by brute‑force search: each integer is tested
 * until the requested term is found.  A sieve of primes up to one
 * million is used to factor candidates efficiently.  The algorithm
 * runs in well under the required time limit for n ≤ 10 000.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility from the available library */
#include "../utils/build_primes_upto.h"

/* --------------------------------------------------------------------- */
/* Static state for the sequence: cache of found terms and list of primes */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;  /* cached sequence values      */
static size_t cache_size = 0;     /* number of cached elements   */
static size_t cache_cap  = 0;     /* current capacity of cache   */
static uint64_t current_candidate = 2;   /* next integer to test      */

static uint64_t *primes   = NULL;   /* list of primes up to 1 000 000 */
static size_t     prime_count = 0;  /* number of primes in the list */

/* --------------------------------------------------------------------- */
/* Helper: grow cache when necessary                                    */
/* --------------------------------------------------------------------- */
static void cache_push(seq_t val)
{
    if (cache_size == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
    }
    cache[cache_size++] = val;
}

/* --------------------------------------------------------------------- */
/* Helper: initialize prime list (once)                                  */
/* --------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes != NULL) return;     /* already initialized */

    /* 1 000 000 is comfortably larger than the square root of the
     * numbers encountered for n ≤ 10 000 (empirically the 10 000th term
     * is < 10⁶).                                            */
    build_primes_upto(1000000ULL, &primes, &prime_count);
}

/* --------------------------------------------------------------------- */
/* Helper: test whether a candidate satisfies the required property      */
/* --------------------------------------------------------------------- */
static bool satisfies(uint64_t X)
{
    uint64_t temp = X;
    int omega = 0;       /* total number of prime factors (with multiplicity) */
    int maxexp = 0;      /* largest exponent in the factorization */

    /* trial division by precomputed primes */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                ++omega;
            }
            if (exp > maxexp)
                maxexp = exp;
        }
    }

    /* If a remaining factor > 1, it is prime */
    if (temp > 1) {
        ++omega;
        if (1 > maxexp)
            maxexp = 1;
    }

    return (omega % 2 == 0) && (maxexp > omega / 2);
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for 0, return 0 defensively */
        return (seq_t)0;

    init_primes();

    /* Generate new terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        uint64_t x = current_candidate++;

        if (satisfies(x))
            cache_push((seq_t)x);
    }

    return cache[n - 1];
}
