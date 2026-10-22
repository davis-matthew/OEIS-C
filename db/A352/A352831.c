/*
 * A352831  Numbers whose weakly increasing prime indices y have exactly one
 *          fixed point y(i) = i.
 * https://oeis.org/A352831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A352831:
   Numbers whose weakly increasing prime indices y have exactly one fixed point y(i) = i. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/build_spf.h"
#include "../utils/build_primes.h"

/* --------------------------------------------------------------------- */
/* Global data --------------------------------------------------------- */
/* --------------------------------------------------------------------- */
static bool          initialized = false;

/* Prime tables */
static idx_t *spf        = NULL;   /* smallest prime factor  */
static idx_t *primes     = NULL;   /* list of primes         */
static size_t  prime_cnt = 0;      /* number of primes       */
static idx_t *prime_to_idx = NULL; /* prime -> 1‑based index */

/* Sequence cache */
static seq_t *seq      = NULL;   /* cache of sequence values */
static size_t seq_cap  = 0;      /* current capacity of seq   */
static size_t seq_len  = 0;      /* number of terms stored    */
static idx_t  next_val = 2;      /* next integer to test      */

/* Limit for sieve; large enough to cover the first 10000 terms. */
#define SIEVE_LIMIT 2000000UL

/* --------------------------------------------------------------------- */
/* Helper: ensure tables are built ------------------------------------- */
/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* build smallest prime factor table up to SIEVE_LIMIT */
    build_spf(SIEVE_LIMIT, &spf);
    assert(spf != NULL);

    /* build list of primes up to SIEVE_LIMIT */
    build_primes(SIEVE_LIMIT, (idx_t **)&primes, &prime_cnt);
    assert(primes != NULL && prime_cnt > 0);

    /* map each prime to its 1‑based index */
    prime_to_idx = (idx_t *)calloc((size_t)(SIEVE_LIMIT + 1), sizeof(idx_t));
    assert(prime_to_idx != NULL);
    for (size_t i = 0; i < prime_cnt; ++i) {
        prime_to_idx[primes[i]] = (idx_t)(i + 1);
    }

    /* allocate initial space for the sequence cache */
    seq_cap = 1024;
    seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    assert(seq != NULL);
}

/* --------------------------------------------------------------------- */
/* Count fixed points in the sorted list of prime indices of n --------- */
/* --------------------------------------------------------------------- */
static bool has_one_fixed_point(idx_t n)
{
    idx_t pos = 1;       /* position in sorted index list (1‑based) */
    int  matches = 0;

    while (n > 1) {
        idx_t p   = spf[n];
        idx_t idx = prime_to_idx[p];   /* index of this prime */

        if (idx == pos) {
            ++matches;
            if (matches > 1)
                return false;          /* early exit: more than one fixed point */
        }
        ++pos;
        n /= p;                         /* remove this factor (handles multiplicity) */
    }
    return matches == 1;
}

/* --------------------------------------------------------------------- */
/* Generate terms until we have at least `need` items in the cache ---- */
/* --------------------------------------------------------------------- */
static void generate_until(size_t need)
{
    while (seq_len < need) {
        /* find next candidate that satisfies the condition */
        idx_t val = next_val;
        ++next_val;  /* increment for next search */

        if (has_one_fixed_point(val)) {
            /* ensure capacity */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                assert(seq != NULL);
            }
            seq[seq_len++] = (seq_t)val;
        } else {
            /* not accepted; simply continue the loop with next_val already incremented */
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public entry point ------------------------------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    assert(n >= 1);
    init_tables();

    /* Ensure we have computed at least n terms. */
    if (n > seq_len)
        generate_until((size_t)n);

    return seq[n - 1];
}
