/*
 * A295598  Numbers k such that Bernoulli number B_{k} has denominator
 *          56786730.
 * https://oeis.org/A295598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A295598 (Numbers k such that Bernoulli number B_k
 *  has denominator 56786730).  A term k must be a multiple of 60, and
 *  for every prime p not dividing 56786730 the integer (p-1)/gcd(p-1,60)
 *  must not divide k/60.  The function below builds the sequence by
 *  sieving these forbidden divisors.  The sequence is generated lazily
 *  and memoised; further terms are produced by expanding the sieve as
 *  needed.  No hard‑coded table of values is used.  The code uses only
 *  integer arithmetic as required by the specification.					*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"   /* declares build_primes_upto */

static seq_t *sequence = NULL;     /* 0‑based array of a(1),a(2),…        */
static size_t seq_len = 0;         /* number of terms stored              */
static size_t seq_cap = 0;         /* allocated capacity                    */
static uint64_t computed_m_limit = 0;  /* largest m processed                */
static bool initialized = false;       /* first‑call flag                      */

/* ------------------------------------------------------------- */
/* Greatest common divisor for 64‑bit values                    */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------- */
/* Check whether p is one of the special primes dividing the
 * denominators   2,3,5,7,11,13,31,61.                               */
static bool is_special_prime(uint64_t p)
{
    switch (p) {
        case 2: case 3: case 5: case 7:
        case 11: case 13: case 31: case 61: return true;
        default: return false;
    }
}

/* ------------------------------------------------------------- */
/* Rebuild the entire sequence up to all m ≤ m_limit (where k = 60*m). */
static void recompute(uint64_t m_limit)
{
    /* free old data if any */
    if (sequence) free(sequence);
    sequence = NULL;
    seq_len  = 0;
    seq_cap  = 0;
    computed_m_limit = m_limit;

    /* 1. Generate all primes up to limit_prime = 60*m_limit + 60 */
    uint64_t limit_prime = 60ULL * m_limit + 60ULL;
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(limit_prime, &primes, &primes_cnt);

    /* 2. Allocate array marking forbidden m's                  */
    unsigned char *invalid = (unsigned char *)calloc((size_t)(m_limit + 1), 1);
    if (!invalid) abort();

    /* 3. For every non‑special prime p, mark multiples of d'    */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (is_special_prime(p)) continue;

        uint64_t g   = gcd64(p - 1ULL, 60ULL);
        uint64_t dpr = (p - 1ULL) / g;          /* d' = (p-1)/gcd(p-1,60) */

        if (dpr == 0 || dpr > m_limit) continue;
        for (uint64_t k = dpr; k <= m_limit; k += dpr)
            invalid[(size_t)k] = 1;
    }

    /* 4. Collect admissible m's into the sequence             */
    seq_cap = 1024;
    sequence = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    if (!sequence) abort();

    for (uint64_t i = 1; i <= m_limit; ++i) {
        if (!invalid[(size_t)i]) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(sequence, seq_cap * sizeof(seq_t));
                if (!tmp) abort();
                sequence = tmp;
            }
            sequence[seq_len++] = (seq_t)(60ULL * i);
        }
    }

    /* 5. Clean up temporary objects                        */
    free(invalid);
    free(primes);
}

/* ------------------------------------------------------------- */
/* Ensure that the global array holds at least n terms. */
static void ensure_terms(idx_t n)
{
    if (!initialized) {
        /* initial estimate: about 80×n + 2000 terms                 */
        uint64_t m_est = (uint64_t)n * 80ULL + 2000ULL;
        if (m_est < 200000ULL) m_est = 200000ULL;
        recompute(m_est);
        initialized = true;
    }

    /* if we still need more terms, enlarge the bound */
    if ((size_t)n > seq_len) {
        uint64_t new_limit = computed_m_limit;
        while ((size_t)n > seq_len) {
            new_limit <<= 1;                 /* double the bound      */
            recompute(new_limit);
        }
    }
}

/* ------------------------------------------------------------- */
/* Public function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but harmless */
    ensure_terms(n);
    return sequence[n - 1];
}
