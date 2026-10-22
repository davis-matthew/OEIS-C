/*
 * A327160  Number of positive integers that are reachable from n with some
 *          combination of transitions x -> usigma(x)-x and x ->
 *          gcd(x,usigma(x)), where usigma is the sum of unitary divisors of n
 *          (A034448).
 * https://oeis.org/A327160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{m > 0 | m is reachable from n by the transitions x ->
 *            usigma(x)-x or x -> gcd(x,usigma(x)) }| (OEIS A327160)
 * Range:     u8 n <= 8807, u16 n <= 8807, u32 n <= 8807, u64 n <= 8807,
 *            u128 n <= 8807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

/* Simple sieve generating all primes up to LIMIT.  This is called once
   on first use of unitary_sigma.  LIMIT is large enough for n ≤ 10000
   and for numbers produced by the transitions. */
static void init_primes(void)
{
    if (primes != NULL) return;          /* already initialized */

    const size_t LIMIT  = 200000;         /* sqrt of largest expected value */
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (is_comp == NULL) return;         /* unlikely */

    primes_cnt = 0;
    primes = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));
    if (primes == NULL) { free(is_comp); return; }

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT)
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
        }
    }
    free(is_comp);
}

/* Sum of unitary divisors of a 64‑bit integer n.
   For each prime power p^e, the unitary divisors are {1, p^e};
   they contribute 1 + p^e to the product. */
static uint64_t unitary_sigma_uint64(uint64_t n)
{
    init_primes();
    uint64_t result = 1;
    uint64_t m = n;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint64_t pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= p;
            }
            result *= (1 + pow);
        }
    }
    if (m > 1) {            /* remaining prime factor > sqrt(n) */
        result *= (1 + m);
    }
    return result;
}

/* Euclidean gcd for uint64_t.  Works in O(log min(a,b)). */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: linear search for value v in array arr[0..len-1].
   Returns true if found, false otherwise. */
static bool contains(const uint64_t *arr, size_t len, uint64_t v)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == v) return true;
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The argument n is guaranteed ≥ 1.  We work in uint64_t for
       all intermediate calculations, since n ≤ 10000 and the
       reachable values never exceed 64 bits for this domain. */
    uint64_t start = (uint64_t)n;

    size_t cap = 16;
    size_t sz  = 0;
    uint64_t *visited = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (visited == NULL) return (seq_t)0;   /* allocation failure */

    visited[sz++] = start;   /* start value is reachable by the empty sequence */

    for (size_t i = 0; i < sz; ++i) {
        uint64_t x  = visited[i];
        uint64_t us = unitary_sigma_uint64(x);

        /* Transition 1: x -> us - x (always > 0 because us > x for x>1) */
        uint64_t t1 = us - x;
        if (t1 > 0 && !contains(visited, sz, t1)) {
            if (sz == cap) {
                cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(visited, cap * sizeof(uint64_t));
                if (tmp == NULL) { free(visited); return (seq_t)sz; }
                visited = tmp;
            }
            visited[sz++] = t1;
        }

        /* Transition 2: x -> gcd(x, us) */
        uint64_t t2 = gcd_u64(x, us);
        if (t2 > 0 && !contains(visited, sz, t2)) {
            if (sz == cap) {
                cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(visited, cap * sizeof(uint64_t));
                if (tmp == NULL) { free(visited); return (seq_t)sz; }
                visited = tmp;
            }
            visited[sz++] = t2;
        }
    }

    free(visited);
    return (seq_t)sz;
}
