/*
 * A374588  Numbers whose maximum exponent in their prime factorization is a
 *          composite number.
 * https://oeis.org/A374588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   computed by scanning natural numbers and selecting those * whose
 *            maximum prime exponent in their factorisation is a composite
 *            number. * A composite exponent is >1 and not prime (2,3 are
 *            excluded)
 * Range:     u8 n <= 12, u16 n <= 3471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9542 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/factor_u64.h"
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

/* Static cache holding the computed terms. */
static seq_t *seq_cache      = NULL;
static size_t seq_cache_len  = 0;   /* number of terms stored */
static size_t seq_cache_cap  = 0;   /* allocated capacity */
static uint64_t next_candidate = 2; /* next integer to test */

/* Ensure the cache contains at least 'n' terms.  This routine scans
 * natural numbers, factorises them and adds the ones whose maximum
 * exponent is composite to the cache. */
static void
ensure_terms(idx_t n)
{
    if (seq_cache == NULL) {
        seq_cache_cap = 16;
        seq_cache = mem_new(seq_t, seq_cache_cap);
    }

    while (seq_cache_len < (size_t)n) {
        uint64_t val = next_candidate++;
        uint64_t primes[64], exponents[64];
        size_t cnt = 0;

        factor_u64(val, primes, exponents, &cnt);

        /* Find the maximum exponent of this integer. */
        uint64_t max_exp = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (exponents[i] > max_exp)
                max_exp = exponents[i];

        /* A composite exponent is >1 and not prime. */
        if (max_exp > 1 && !isPrimeGeneric((seq_t)max_exp)) {
            /* Grow cache if necessary. */
            if (seq_cache_len == seq_cache_cap) {
                seq_cache_cap *= 2;
                seq_cache = mem_resize(seq_t, seq_cache, seq_cache_cap);
            }
            seq_cache[seq_cache_len++] = (seq_t)val;
        }
    }
}

/* Public routine that returns the nth term of the sequence.  The
 * sequence is 1‑based (a(1) is the first listed term). */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but return 0 for safety */
    ensure_terms(n);
    return seq_cache[n - 1];
}
