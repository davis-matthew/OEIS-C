/*
 * A243111  Difference between the smallest triangular number >= n-th prime
 *          and the n-th prime.
 * https://oeis.org/A243111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{ T_k - p_n | T_k triangular >= p_n }
 * Range:     u8 n <= 4, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_n is the n‑th prime and T_k = k*(k+1)/2 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

static uint64_t *primes_u64 = NULL;   /* array of primes (uint64_t) */
static size_t      primes_cnt = 0;    /* number of primes stored */
static size_t      primes_cap = 0;    /* capacity of the primes array */
static uint64_t    last_prime  = 1;    /* last prime generated */

/* Ensure the primes array has capacity at least "cap". */
static void ensure_capacity(size_t cap)
{
    if (primes_cap >= cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    if (new_cap < cap) new_cap = cap;
    uint64_t *tmp = (uint64_t*)realloc(primes_u64, new_cap * sizeof(uint64_t));
    if (!tmp) { /* allocation failure – caller will crash */ }
    primes_u64 = tmp;
    primes_cap = new_cap;
}

/* Add a new prime to the array. */
static void add_prime(uint64_t p)
{
    ensure_capacity(primes_cnt + 1);
    primes_u64[primes_cnt++] = p;
    last_prime = p;
}

/* Generate primes until we have at least "need" primes. */
static void generate_primes_to(size_t need)
{
    while (primes_cnt < need) {
        uint64_t nxt = next_prime(last_prime);
        add_prime(nxt);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, guard for safety */
    generate_primes_to((size_t)n);

    /* n-th prime (1‑based indexing). */
    uint64_t p64 = primes_u64[n - 1];
    seq_t p = (seq_t)p64;                    /* cast to seq_t for calculation */

    /* temp = 1 + 8*p   (fits in seq_t for the ranges tested). */
    seq_t temp = (seq_t)1 + (seq_t)8 * p;

    /* s = floor(sqrt(temp)) */
    seq_t s = floor_sqrt(temp);

    /* initial k = floor((s-1)/2) */
    seq_t k = (s - (seq_t)1) / (seq_t)2;

    /* compute triangular number T_k */
    seq_t t = k * (k + (seq_t)1) / (seq_t)2;

    /* if T_k < p, bump k by one and recompute */
    if (t < p) {
        k = k + (seq_t)1;
        t = k * (k + (seq_t)1) / (seq_t)2;
    }

    return t - p;       /* minimal triangular number minus the prime */
}
