/*
 * A274435  Numbers having equal numbers of distinct prime factors of forms
 *          3*k+1 and 3*k+2.
 * https://oeis.org/A274435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A274435: Numbers having equal numbers of distinct prime factors of the
 *   forms 3*k+1 and 3*k+2.
 *
 * This implementation does not use a closed formula; instead it
 * generates terms lazily, keeping a cache and a smallest‑prime‑factor (SPF)
 * array that is expanded on demand.  Each integer is tested by factorising
 * via the SPF array, counting distinct primes in the two residue classes,
 * and accepting it when the counts are equal (including the special case
 * n = 1, which has no prime factors of either type).
 *
 * The algorithm is O(n log log n) for the first n terms, which is easily
 * fast enough for n up to at least 10 000.  All arithmetic for the
 * candidate values uses 64‑bit unsigned integers (uint64_t); the final
 * result is converted to the build‑specific type seq_t.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*                            CACHE                               */
/* ------------------------------------------------------------ */

/* Cache of found terms as uint64_t – they fit easily in 64 bits for
 * all tests up to n = 10 000.  The array holds the first terms;
 * a(idx_t n) indexes it with (n-1). */
static uint64_t *terms      = NULL;
static size_t   terms_cnt   = 0;
static size_t   terms_cap   = 0;

/* Grow the terms cache to accommodate at least 'needed' entries. */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                     /* out‑of‑memory: terminate */
    terms     = tmp;
    terms_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*                         SMALL‑PRIME‑FACTOR                    */
/* ------------------------------------------------------------ */

/* SPF array: spf[i] is the smallest prime factor of i.
 * spf[0] and spf[1] remain zero.  The array grows by doubling
 * its size on demand. */
static uint64_t *spf             = NULL;
static uint64_t  spf_size        = 0;     /* array length (highest index + 1) */
static uint64_t  spf_limit       = 0;     /* maximum n for which SPF is valid */

/* Rebuild SPF table up to 'limit' (inclusive).  A full recomputation
 * is performed; this is called only when the table size is doubled. */
static void rebuild_spf_up_to(uint64_t limit)
{
    uint64_t *new_spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_spf) abort();
    for (uint64_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {              /* avoid overflow */
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf      = new_spf;
    spf_size = limit + 1;
}

/* Ensure SPF table covers at least the value 'n'.  The table is
 * expanded by a factor of two each time it runs out of space. */
static void ensure_spf_up_to(uint64_t n)
{
    if (spf_limit >= n) return;               /* already sufficient */

    uint64_t new_limit;
    if (spf_limit == 0) new_limit = 2;
    else                new_limit = spf_limit * 2 + 10;  /* small safety margin */

    if (new_limit < n) new_limit = n + 10;
    if (new_limit < n) new_limit = n;          /* in case of overflow */

    rebuild_spf_up_to(new_limit);
    spf_limit = new_limit - 1;                 /* inclusive max index */
}

/* ------------------------------------------------------------ */
/*                 EQUAL MOD COUNT CHECK (core test)           */
/* ------------------------------------------------------------ */

/* Returns true iff the number of distinct prime factors of n that
 * are ≡1 (mod 3) equals the number that are ≡2 (mod 3).  The prime
 * factor 3 itself is ignored, as it is ≡0 (mod 3).  The special
 * case n=1 returns true (zero primes of either type). */
static bool has_equal_mod_counts(uint64_t n)
{
    if (n == 1) return true;                  /* 1 has zero primes of each type */

    ensure_spf_up_to(n);                      /* make sure SPF is ready */

    uint64_t x = n;
    int cnt1 = 0, cnt2 = 0;

    while (x > 1) {
        uint64_t p = spf[x];
        if (p % 3 == 1) ++cnt1;
        else if (p % 3 == 2) ++cnt2;
        /* skip all powers of p */
        while (x % p == 0) x /= p;
    }
    return cnt1 == cnt2;
}

/* ------------------------------------------------------------ */
/*                    GENERATE TERMS ON DEMAND                 */
/* ------------------------------------------------------------ */

static void generate_terms_up_to(idx_t n)
{
    if (n <= (idx_t)terms_cnt) return;        /* already available */

    uint64_t cand = terms_cnt ? terms[terms_cnt - 1] + 1 : 1;

    while ((idx_t)terms_cnt < n) {
        if (has_equal_mod_counts(cand)) {
            grow_terms(terms_cnt + 1);
            terms[terms_cnt++] = cand;
        }
        ++cand;
    }
}

/* ------------------------------------------------------------ */
/*                      PUBLIC INTERFACE                        */
/* ------------------------------------------------------------ */

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* OAOS offset is 1, undefined for 0 */

    generate_terms_up_to(n);
    return (seq_t)terms[n - 1];
}
