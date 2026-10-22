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
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/isPrimeGeneric.h"

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
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
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
                seq_cache = (seq_t *)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
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
