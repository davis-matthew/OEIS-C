/*
 * A318841  a(n) = n - A173557(n).
 * https://oeis.org/A318841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - product of (p-1) over distinct primes dividing n
 * Range:     u8 n <= 269, u16 n <= 9112, u32 n <= 9112, u64 n <= 9112,
 *            u128 n <= 9112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Smallest‑prime‑factor sieve, grown lazily to cover requested values. */
static idx_t _spf_limit = 0;           /* largest n for which the sieve is built. */
static uint32_t *_spf = NULL;          /* array of smallest prime factors. */

/* Ensures that _spf contains SPF for every integer up to at least max_n. */
static void _ensure_spf(idx_t max_n)
{
    if (max_n <= _spf_limit)
        return;                         /* already sufficient */

    /* Allocate a new array large enough. */
    size_t sz = (size_t)max_n + 1;
    uint32_t *new_spf = (uint32_t*)malloc(sz * sizeof(uint32_t));
    if (!new_spf)
        exit(EXIT_FAILURE);             /* fatal error on allocation failure */

    /* Initialise SPF array: spf[i] = i. */
    for (size_t i = 0; i < sz; ++i)
        new_spf[i] = (uint32_t)i;

    /* Classic sieve to compute the smallest prime factor for each number. */
    for (uint32_t i = 2; (uint64_t)i * (uint64_t)i <= max_n; ++i) {
        if (new_spf[i] == i) {          /* i is prime */
            for (uint64_t j = (uint64_t)i * (uint64_t)i;
                 j <= max_n; j += i) {
                if (new_spf[(size_t)j] == (uint32_t)j)
                    new_spf[(size_t)j] = i;
            }
        }
    }

    /* Replace the old sieve with the new one. */
    free(_spf);
    _spf = new_spf;
    _spf_limit = max_n;
}

/* Sequence A318841:  a(n) = n - ∏_{p|n} (p-1)   (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for OEIS offset, return 0 safely. */
        return (seq_t)0;

    /* Make sure we have prime factors for every number up to n. */
    _ensure_spf(n);

    seq_t product = (seq_t)1;   /* Running product of (p-1) for distinct primes. */
    idx_t temp = n;

    if (temp > 1) {
        while (temp > 1) {
            uint32_t p = _spf[(size_t)temp];
            product *= (seq_t)(p - 1);
            while (temp % p == 0)
                temp /= p;
        }
    }

    return (seq_t)n - product;
}
