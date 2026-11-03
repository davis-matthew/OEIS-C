/*
 * A351387  Primes p such that if q is the next prime, (p+q)/5+5 is prime.
 * https://oeis.org/A351387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9896 (2026-11-03)
 * Helpers:   build_primes, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351387: primes p for which q = next prime satisfies
   (p+q)/5 + 5 is prime.  The function scans successive primes and
   records those that meet the condition.  It works for any
   seq_t type supported by the utility functions. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 is undefined */

    static bool initialized = false;
    static idx_t *primes = NULL;
    static size_t prime_count = 0;
    static seq_t *terms = NULL;
    static size_t term_cap = 0;
    static size_t term_count = 0;
    static size_t prime_idx = 0;

    if (!initialized)
    {
        /* Generate a generous list of primes.
           200 000 000 is sufficient to produce thousands of terms. */
        const idx_t LIMIT = 200000000ULL;
        build_primes(LIMIT, &primes, &prime_count);

        term_cap = 1024;
        terms = mem_new(seq_t, term_cap);
        term_count = 0;
        prime_idx  = 0;
        initialized = true;
    }

    /* Convert n to 0‑based index for internal arrays */
    const size_t idx = (size_t)(n - 1);

    /* Produce more terms until we have at least n of them. */
    while (term_count <= idx && prime_idx + 1 < prime_count)
    {
        const uint64_t p = primes[prime_idx];
        const uint64_t q = primes[prime_idx + 1];
        const uint64_t sum = p + q;

        if ((sum % 5ULL) == 0ULL)
        {
            const seq_t val = (seq_t)(sum / 5ULL + 5ULL);
            if (isPrimeGeneric(val))
            {
                /* Store the qualifying prime p. */
                if (term_count == term_cap)
                {
                    term_cap *= 2;
                    terms = mem_resize(seq_t, terms, term_cap);
                }
                terms[term_count++] = (seq_t)p;
            }
        }
        prime_idx++;
    }

    /* If we exhausted our pre‑computed primes, return 0 as a sentinel. */
    if (term_count <= idx) return 0;

    return terms[idx];
}
