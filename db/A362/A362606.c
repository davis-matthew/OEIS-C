/*
 * A362606  Numbers without a unique least prime exponent, or numbers whose
 *          prime factorization has more than one element of least
 *          multiplicity.
 * https://oeis.org/A362606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A362606: first term is 6, symbols for the nth smallest integer
   whose prime factorization has a least exponent occurring at least twice.
   Algorithm: scan integers starting at 2, test the factorization by
   simple trial division, keep all qualifying numbers in a cache. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Helper: test whether n satisfies the condition ---------- */
static bool qualifies(uint64_t n)
{
    uint64_t temp = n;
    uint64_t min_exp = 0;
    size_t min_count = 0;
    bool first = true;

    /* factor 2 */
    if ((temp & 1) == 0) {
        uint64_t exp = 0;
        do {
            temp >>= 1;   /* temp /= 2 */
            ++exp;
        } while ((temp & 1) == 0);
        if (first) {
            min_exp = exp;
            min_count = 1;
            first = false;
        } else {
            if (exp < min_exp) {
                min_exp = exp;
                min_count = 1;
            } else if (exp == min_exp) {
                ++min_count;
            }
        }
    }

    /* factor odd primes by trial division */
    for (uint64_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            uint64_t exp = 0;
            do {
                temp /= d;
                ++exp;
            } while (temp % d == 0);
            if (first) {
                min_exp = exp;
                min_count = 1;
                first = false;
            } else {
                if (exp < min_exp) {
                    min_exp = exp;
                    min_count = 1;
                } else if (exp == min_exp) {
                    ++min_count;
                }
            }
        }
    }

    /* remaining prime factor > sqrt(original) */
    if (temp > 1) {
        uint64_t exp = 1;
        if (first) {
            min_exp = exp;
            min_count = 1;
            first = false;
        } else {
            if (exp < min_exp) {
                min_exp = exp;
                min_count = 1;
            } else if (exp == min_exp) {
                ++min_count;
            }
        }
    }

    /* first is always false for n >= 2, guard just in case */
    return !first && min_count >= 2;
}

/* ---------- Cache management ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of cache */
static uint64_t next_to_test = 2;  /* next integer to examine */

/* Ensure cache has at least 'needed' slots */
static void ensure_cache_size(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t newcap = (cache_cap == 0) ? 16 : cache_cap;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newp)
        abort();
    cache = newp;
    cache_cap = newcap;
}

/* ---------- Sequence generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, return 0 as a fallback */

    size_t required = (size_t)n;
    ensure_cache_size(required);

    /* Generate terms until we have at least 'required' of them. */
    while (cache_len < required) {
        if (qualifies(next_to_test)) {
            cache[cache_len] = (seq_t)next_to_test;
            ++cache_len;
        }
        ++next_to_test;
    }

    return cache[n - 1];
}
