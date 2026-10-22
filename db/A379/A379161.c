/*
 * A379161  Primes p such that the multiplicative order of 7 modulo p is
 *          prime.
 * https://oeis.org/A379161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 520, u32 n <= 1676, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A379161: primes p such that the multiplicative order
   of 7 modulo p is prime.  The algorithm sieves primes up to an
   adaptive limit, tests each prime by computing the order of 7 modulo
   that prime, checks whether the order is prime, and stores the prime
   in a memoized array.  The function works for any build where
   seq_t is an unsigned integer type or an arbitrary‑precision class. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/powmod64.h"

/* --- static state ------------------------------------------------------- */
static uint64_t *primes = NULL;          /* list of primes up to sieve_limit */
static size_t   prime_cnt = 0;           /* number of primes stored       */
static idx_t    sieve_limit = 2000000;   /* current sieve bound           */
static size_t   next_prime_index = 0;    /* next prime to inspect         */

static seq_t *results = NULL;           /* memoized sequence values      */
static size_t results_cap = 0;           /* capacity of results array     */
static size_t results_size = 0;          /* number of terms found         */
static bool     initialized = false;     /* initialization flag           */

/* ----------------------------------------------------------------------- */
/* Extend the prime sieve to at least “limit”.  If “limit” is larger
   than the current bound, rebuild the prime list.                       */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit && primes != NULL)
        return;                         /* already fine */

    free(primes);
    primes = NULL;
    prime_cnt = 0;

    build_primes_upto(limit, &primes, &prime_cnt);
    sieve_limit = limit;
}

/* Ensure the results array can hold at least “need” more entries. */
static void ensure_results_cap(size_t need)
{
    if (results_cap >= results_size + need)
        return;

    size_t new_cap = (results_cap == 0) ? 64 : results_cap * 2;
    while (new_cap < results_size + need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failure */

    results = tmp;
    results_cap = new_cap;
}

/* Compute multiplicative order of 7 modulo prime p (p is odd prime). */
static uint64_t multiplicative_order(uint64_t p)
{
    uint64_t ord = p - 1;
    uint64_t temp = ord;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t q = primes[i];
        if (temp % q != 0)
            continue;

        /* Remove q from temp completely. */
        while (temp % q == 0)
            temp /= q;

        /* Reduce ord by powers of q while the condition holds. */
        while (ord % q == 0 && powmod64(7ULL, ord / q, p) == 1ULL)
            ord /= q;
    }

    /* Remaining prime factor > 1. */
    if (temp > 1) {
        uint32_t q = (uint32_t)temp;
        while (ord % q == 0 && powmod64(7ULL, ord / q, p) == 1ULL)
            ord /= q;
    }

    return ord;
}

/* One-time initialization of static structures. */
static void init(void)
{
    if (initialized)
        return;

    extend_sieve(sieve_limit);
    results = NULL;
    results_cap = 0;
    results_size = 0;
    next_prime_index = 0;
    initialized = true;
}

/* ----------------------------------------------------------------------- */
/* Main function required by the OEIS framework.                           */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* index 0 is undefined */

    init();

    /* Generate terms until we have at least n of them. */
    while (results_size < (size_t)n) {
        /* If we have exhausted the current prime list, extend the sieve. */
        if (next_prime_index >= prime_cnt) {
            extend_sieve(sieve_limit * 2);
            continue;                   /* re-enter loop to use new primes */
        }

        uint64_t p = primes[next_prime_index++];
        if (p == 2)                       /* skip the only even prime */
            continue;

        uint64_t ord = multiplicative_order(p);

        if (is_prime_big((seq_t)ord)) {
            ensure_results_cap(1);
            results[results_size++] = (seq_t)p;
        }
    }

    return results[n - 1];               /* 1‑based indexing */
}
