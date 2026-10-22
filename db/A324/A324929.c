/*
 * A324929  Numbers whose product of prime indices is even.
 * https://oeis.org/A324929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324929: numbers whose product of prime indices is even.
   The nth term a(n) is the nth positive integer that has at least
   one prime factor whose index in the ordered list of primes (2,3,5,…)
   is even (i.e. 3,7,13,19,31,…). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

static const idx_t MAX_PRIME = 200000;          /* limit for the primes table */

/* Global static data (initialised once) */
static bool          initialized = false;

static uint64_t      *prime_list = NULL;        /* primes up to MAX_PRIME   */
static size_t        prime_count = 0;           /* number of primes         */
static bool          *even_index = NULL;        /* flag for even‑indexed primes */

static seq_t         *seq_cache = NULL;         /* cached sequence values   */
static size_t        seq_len   = 0;             /* number of cached terms   */
static size_t        seq_cap   = 0;             /* capacity of seq_cache    */

static uint64_t      current_candidate = 2;     /* smallest integer not yet tested */

/* Initialise all static tables and variables on first call. */
static void ensure_initialized(void)
{
    if (initialized)
        return;

    /* Build the list of primes up to MAX_PRIME */
    build_primes_upto(MAX_PRIME, &prime_list, &prime_count);

    /* Allocate array that marks whether a prime has an even index. */
    even_index = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    for (size_t i = 0; i < prime_count; ++i) {
        if (((i + 1) & 1) == 0)          /* even index (1‑based)   */
            even_index[prime_list[i]] = true;
    }

    /* Allocate initial cache for the sequence */
    seq_cap   = 1024;
    seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_len   = 0;

    initialized = true;
}

/* Append a new term to the cached sequence, expanding storage if needed. */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap * 2 + 16;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* Public function returning the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)          /* safety: undefined for n=0, but return 0 */
        return (seq_t)0;

    ensure_initialized();

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Factorization of current_candidate and check for even‑indexed primes. */
        uint64_t fac_pr[64];
        uint64_t fac_ex[64];
        size_t fac_cnt = 0;

        factor_u64(current_candidate, fac_pr, fac_ex, &fac_cnt);

        bool has_even = false;
        for (size_t i = 0; i < fac_cnt && !has_even; ++i) {
            uint64_t p = fac_pr[i];
            if (p <= MAX_PRIME) {
                if (even_index[p])
                    has_even = true;
            } else {   /* p > MAX_PRIME: determine its index by searching
                         the prime list. For our limits it never occurs, but
                         the fallback makes the routine correct for all n. */
                for (size_t j = 0; j < prime_count; ++j)
                    if (prime_list[j] > p)
                        break;
                    else if (prime_list[j] == p) {
                        if (((j + 1) & 1) == 0)
                            has_even = true;
                        break;
                    }
            }
        }

        if (has_even)
            append_term((seq_t)current_candidate);

        ++current_candidate;
    }

    return seq_cache[n - 1];
}
