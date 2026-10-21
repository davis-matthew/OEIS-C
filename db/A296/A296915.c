/*
 * A296915  Primes that are not squares mod 163.
 * https://oeis.org/A296915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p (p≠163) such that JacobiSymbol(p,163) =
 *            -1, i.e. inert primes in ℚ(√‑163)
 * Range:     u8 n <= 34, u16 n <= 3297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */

static seq_t *cache = NULL;   /* stored primes that satisfy the condition */
static size_t cache_len = 0;   /* how many primes are currently stored */
static size_t cache_cap = 0;   /* current allocation size */
static uint64_t last_checked = 1;   /* we start searching from 2 */

/* compute Jacobi symbol (a|n). n must be an odd positive integer. */
static int jacobi_symbol(uint64_t a, uint64_t n)
{
    if (n == 0) return 0;
    a %= n;
    int result = 1;
    while (a != 0) {
        while ((a & 1) == 0) {           /* while a even */
            a >>= 1;
            uint64_t m = n & 7u;          /* n mod 8 */
            if (m == 3u || m == 5u) result = -result;
        }
        /* swap a and n */
        uint64_t temp = a; a = n; n = temp;
        if ((a & 3) == 3 && (n & 3) == 3) result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

/* append a prime value to the cache, growing storage if necessary */
static void append_prime(seq_t p)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 safely */

    /* generate enough primes until we have n entries in the cache */
    while (cache_len < (size_t)n) {
        uint64_t p = last_checked + 1;

        /* find next prime greater than last_checked */
        while (!is_prime(p)) ++p;
        last_checked = p;

        if (p == 163ULL) continue;                   /* skip ramified prime */

        /* test Jacobi symbol */
        if (jacobi_symbol(p, 163ULL) == -1)
            append_prime((seq_t)p);
    }
    return cache[(size_t)n - 1];
}
