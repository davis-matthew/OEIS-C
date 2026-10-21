/*
 * A254923  The slowest increasing sequence of semiprimes with alternating
 *          parity.
 * https://oeis.org/A254923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest semiprime > a(n-1) such that a(n) and
 *            a(n-1) have opposite parity
 * Range:     u8 n <= 55, u16 n <= 6400, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A254923   Offset: 1   (a(1) = 4) */

/* Standard library headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global variables for the sequence and the SPF sieve */
static seq_t *seq = NULL;          /* memoized sequence values */
static size_t seq_len = 0;         /* number of values stored (seq_len>=1 after init) */
static size_t seq_cap = 0;         /* capacity of the seq array */

static uint32_t *spf = NULL;       /* smallest prime factor table */
static uint64_t spf_limit = 0;     /* maximum index covered by spf */

/* Build an SPF sieve up to <limit> (inclusive).  Must be called with limit>=2. */
static void build_spf(uint64_t limit)
{
    /* Allocate new table.  We use malloc instead of a helper to avoid external dependencies. */
    uint32_t *new_spf = (uint32_t*)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (new_spf == NULL) abort();

    /* Sieve for smallest prime factor */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {        /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i > limit / i) continue;  /* avoid overflow */
            for (uint64_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* Replace old table */
    if (spf != NULL) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Ensure the SPF table covers at least <limit> (inclusive). */
static void ensure_sieve_limit(uint64_t limit)
{
    if (limit <= spf_limit) return;           /* already sufficient */

    /* Grow to the next power of two >= limit, unless we already have a table. */
    uint64_t new_limit = spf_limit ? spf_limit : 1 << 17;   /* start with 131072 if first time */
    while (new_limit < limit) new_limit <<= 1;
    build_spf(new_limit);
}

/* Test whether x is a semiprime.  Requires spf table to cover x. */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;
    if (x > spf_limit) return false;          /* should not happen if ensure_sieve_limit was called */

    uint32_t p = spf[(size_t)x];
    if (p == 0 || p == (uint32_t)x) return false;   /* prime or 1 */

    uint64_t q = x / p;                         /* second factor */
    return spf[(size_t)q] == q;                 /* q is prime if its spf is itself */
}

/* Ensure the internal sequence buffer can hold at least <needed> elements. */
static void seq_ensure(size_t needed)
{
    if (needed <= seq_cap) return;

    size_t new_cap = seq_cap ? (seq_cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();   /* out of memory */

    seq = tmp;
    seq_cap = new_cap;
}

/* Main function for the OEIS sequence A254923 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Lazily initialize the sequence cache with the first term. */
    if (!seq) seq_ensure(8);              /* some default capacity */
    if (seq_len == 0) {
        seq[0] = (seq_t)4;
        seq_len = 1;
    }

    /* Ensure sieve table covers the initial candidates. */
    if (!spf) ensure_sieve_limit(200000);

    /* Generate terms up to the requested index. */
    while ((idx_t)seq_len < n) {
        seq_t prev_term = seq[seq_len - 1];
        uint64_t prev = (uint64_t)prev_term;   /* safe cast – values are expected to fit in 64‑bit */
        uint64_t cand = prev + 1;

        /* Expand the sieve if needed for the next candidate. */
        ensure_sieve_limit(cand);

        /* Search for the next term with alternating parity and semiprime property. */
        while (1) {
            if ( (cand & 1) == (prev & 1) ) { /* not alternating parity */
                ++cand;
            } else if (!is_semiprime(cand)) {  /* not a semiprime */
                ++cand;
            } else {                           /* found the next term */
                break;
            }
            if (cand > spf_limit) ensure_sieve_limit(cand);
        }

        /* Store the found term. */
        seq_ensure(seq_len + 1);
        seq[seq_len] = (seq_t)cand;
        ++seq_len;
    }

    return seq[n - 1];
}
