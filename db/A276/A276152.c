/*
 * A276152  a(n) = {smallest prime not dividing n} times {greatest primorial
 *          number which divides n} = A053669(n) * A053589(n).
 * https://oeis.org/A276152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053589(n) * A053669(n) where A053589(n) = greatest
 *            primorial dividing n and A053669(n) = smallest prime not
 *            dividing n
 * Range:     u8 n <= 209, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Prime sieve (once per program run).                                 */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 200000      /* sufficient for n up to at least 1e8        */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Lazy initialization of the prime table */
static void init_primes(void)
{
    if (primes != NULL)
        return;                    /* already initialized */

    /* Allocate a boolean array for the sieve */
    bool *is_p = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (is_p == NULL)
        exit(EXIT_FAILURE);

    /* Initialize the sieve */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_p[i] = true;
    is_p[0] = is_p[1] = false;

    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_p[i]) {
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_p[j] = false;
        }
    }

    /* Count primes to allocate exact space */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_p[i])
            ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (primes == NULL)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_p[i])
            primes[idx++] = (uint64_t)i;

    prime_cnt = cnt;
    free(is_p);
}

/* --------------------------------------------------------------------- */
/*  Helper to find the smallest prime not dividing n.                    */
/* --------------------------------------------------------------------- */
static uint64_t smallest_nondivisor_prime(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (n % p != 0)
            return p;
    }

    /* If we exhaust the precomputed primes, generate further ones */
    uint64_t candidate = primes[prime_cnt - 1] + 1;
    while (candidate % 2 == 0)
        ++candidate;      /* skip even numbers */

    for (;; ++candidate) {
        if (candidate % 2 != 0) {
            bool is_prime = true;
            for (uint64_t d = 3; d * d <= candidate; d += 2) {
                if (candidate % d == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (n % candidate != 0)
                    return candidate;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Helper to find the greatest primorial dividing n.                    */
/* --------------------------------------------------------------------- */
static uint64_t greatest_primorial_dividing(uint64_t n)
{
    uint64_t primorial = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t candidate = primorial * p;
        if (candidate > n)
            break;          /* further products will only be larger */

        if (n % candidate == 0) {
            primorial = candidate;   /* extend the primorial */
        } else {
            break;          /* cannot extend further */
        }
    }
    return primorial;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure the prime table is ready. */
    init_primes();

    /* Treat n as an unsigned 64‑bit integer for arithmetic. */
    uint64_t n64 = (uint64_t)n;

    /* Compute the two factors. */
    uint64_t primorial = greatest_primorial_dividing(n64);
    uint64_t small     = smallest_nondivisor_prime(n64);

    /* Return the product, cast to the appropriate type. */
    return (seq_t)(primorial * small);
}
