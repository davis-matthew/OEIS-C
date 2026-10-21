/*
 * A256737  Number of composites lying between successive pairs of primes,
 *          beginning with pair (2,3). Bisection of A046933.
 * https://oeis.org/A256737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256737 : number of composites between successive primes (pair (2,3) is first). */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeTrial.h"

/* static cache of primes */
static uint64_t *primes = NULL;   /* array of discovered primes (stored as uint64_t) */
static size_t prime_count = 0;    /* number of primes currently stored */
static size_t prime_capacity = 0; /* allocated capacity of primes array */

/* grow the primes array to at least new_cap entries */
static void grow_primes_array(size_t new_cap)
{
    if (new_cap <= prime_capacity)
        return;
    size_t cap = prime_capacity ? prime_capacity : 16;
    while (cap < new_cap) cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(*primes));
    if (!tmp) abort();            /* out‑of‑memory abort, as per typical oeis build policy */
    primes = tmp;
    prime_capacity = cap;
}

/* test whether a 64‑bit integer is prime (trial division up to sqrt).  */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    while (i * i <= n)
    {
        if (n % i == 0   || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

/* ensure that at least need primes are available in the cache */
static void ensure_primes_up_to(size_t need)
{
    if (prime_count >= need)
        return;

    grow_primes_array(need);
    uint64_t cand = prime_count ? primes[prime_count - 1] + 1 : 2;
    while (prime_count < need)
    {
        if (is_prime_uint64(cand))      /* use our own routine (fast enough for 200k limit) */
            primes[prime_count++] = cand;
        cand++;
    }
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety; OEIS offset is 1 */

    /* we need the (2n)-th prime */
    size_t need = (size_t)(2 * n);
    ensure_primes_up_to(need);

    /* primes are 0‑based indexed: primes[0] = 2, primes[1] = 3, ... */
    uint64_t p1 = primes[2 * n - 2];
    uint64_t p2 = primes[2 * n - 1];

    /* composites between successive primes = all numbers in (p1,p2) minus 0 primes inside */
    uint64_t composites = p2 > p1 ? (p2 - p1 - 1) : 0;

    return (seq_t)composites;
}
