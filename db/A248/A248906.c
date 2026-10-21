/*
 * A248906  Binary representation of prime power divisors of n: Sum_{p^k | n}
 *          2^(A025528(p^k)-1).
 * https://oeis.org/A248906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{p^k | n} 2^(index(p^k)-1) for all prime powers = p^k
 *            dividing n, where index(p^k) is the 1‑based position of p^k in
 *            the ordered list of all prime powers
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 82, u64 n <= 232,
 *            u128 n <= 592, big any n
 * Helpers:   build_primes_upto, factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

static uint64_t *prime_powers = NULL;
static size_t prime_powers_cnt = 0;
static size_t prime_powers_cap = 0;
static idx_t cached_max_n = 0; /* largest n for which prime_powers is valid */

/* compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* rebuild prime_powers table up to n */
static void rebuild_prime_powers(idx_t n)
{
    /* generate all primes <= n */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n, &primes, &primes_cnt);

    /* estimate capacity (roughly twice the number of primes suffices) */
    size_t est_cap = primes_cnt * 2;
    if (prime_powers_cap < est_cap) {
        free(prime_powers);
        prime_powers = (uint64_t *)malloc(est_cap * sizeof(uint64_t));
        prime_powers_cap = est_cap;
    }

    /* collect all prime powers <= n */
    size_t idx = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t val = p;
        while (val <= n) {
            if (idx >= prime_powers_cap) {
                prime_powers_cap *= 2;
                prime_powers = (uint64_t *)realloc(prime_powers,
                                                  prime_powers_cap * sizeof(uint64_t));
            }
            prime_powers[idx++] = val;
            if (val > n / p) break; /* next multiplication would overflow or exceed n */
            val *= p;
        }
    }

    /* sort the prime powers */
    qsort(prime_powers, idx, sizeof(uint64_t), cmp_uint64);
    prime_powers_cnt = idx;
    cached_max_n = n;
    free(primes);
}

/* binary search for the 1‑based index of a prime power in prime_powers */
static idx_t index_in_prime_powers(uint64_t val)
{
    size_t lo = 0, hi = prime_powers_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t midval = prime_powers[mid];
        if (midval == val)
            return (idx_t)(mid + 1); /* 1‑based index */
        if (midval < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* should never happen for valid prime powers */
}

/* required function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n=0, harmless */

    /* ensure prime_powers table covers n */
    if (n > cached_max_n)
        rebuild_prime_powers(n);

    /* factor n into primes and exponents */
    uint64_t pf[64], ex[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, pf, ex, &pcnt);

    seq_t sum = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pf[i];
        uint64_t val = 1;
        for (uint64_t k = 1; k <= ex[i]; ++k) {
            val *= p; /* p^k <= n */
            idx_t idx = index_in_prime_powers(val);
            sum += pow2(idx - 1); /* 2^(idx-1) */
        }
    }
    return sum;
}
