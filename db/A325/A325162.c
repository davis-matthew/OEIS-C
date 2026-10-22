/*
 * A325162  Squarefree numbers with no two prime indices differing by less
 *          than 3.
 * https://oeis.org/A325162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A325162:
   Squarefree numbers whose prime indices differ by at least 3.
   This implementation generates the sequence by testing successive
   integers and storing every qualifying value.  It allocates a fixed
   sieve up to a generous limit (200 000) and a static array large
   enough to hold more than 10 000 terms.  The routine works for both
   ordinary integer types and the arbitrary‑precision build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_t *seq_alloc_new_array(size_t capacity); */

/* Smallest prime factor array and prime‑index table.  They are built
   once and never resized. */
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i (i <= sieve_limit) */
static uint32_t *prime_index = NULL;/* 1‑based index of p for prime p, 0 otherwise */
static idx_t sieve_limit = 0;      /* limit up to which spf and prime_index are valid */

/* Sequence cache.  We allocate enough capacity for over 20 000 terms
   which comfortably covers n ≤ 10 000. */
static seq_t *terms = NULL;       /* array of established terms (0‑based) */
static size_t terms_len = 0;      /* number of terms currently stored */
static const size_t TERMS_CAP = 20000;  /* static capacity */

/* Next integer to test for membership. */
static idx_t current_candidate = 1;

/* ---- Sieve construction ------------------------------------------------- */
/* Build spf and prime_index arrays up to 'limit'.  The function assumes
   that no larger limit will be requested later.  It allocates memory
   for both arrays and fills them. */
static void build_sieve(idx_t limit)
{
    sieve_limit = limit;

    /* Allocate smallest prime factor array */
    spf = (idx_t *)malloc((size_t)(sieve_limit + 1) * sizeof(idx_t));
    if (!spf) abort();

    /* Allocate prime‑index lookup array */
    prime_index = (uint32_t *)malloc((size_t)(sieve_limit + 1) * sizeof(uint32_t));
    if (!prime_index) abort();

    /* Initialise spf array: spf[i] = i */
    for (idx_t i = 0; i <= sieve_limit; ++i) {
        spf[i] = i;
        prime_index[i] = 0;              /* default: not a prime */
    }
    if (sieve_limit >= 0) spf[0] = 0;
    if (sieve_limit >= 1) spf[1] = 1;

    /* Classic sieve for smallest prime factors */
    for (idx_t i = 2; i * i <= sieve_limit; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            for (idx_t j = i * i; j <= sieve_limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* Build prime index table */
    uint32_t idx = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            ++idx;
            prime_index[(uint32_t)i] = idx;
        }
    }
}

/* ---- Candidate test --------------------------------------------------- */
/* Return true iff x is squarefree and the indices of its prime factors
   differ pairwise by at least 3.  The algorithm factors x using the
   pre‑computed spf array. */
static bool good_candidate(idx_t x)
{
    idx_t n = x;
    uint32_t indices[32];        /* great enough for any 64‑bit integer */
    size_t num_factors = 0;

    while (n > 1) {
        idx_t p = spf[n];
        if (p == 0) p = n;          /* safety: should never happen */

        /* Squarefree check: ensure p² ∤ n */
        if ((n / p) % p == 0) return false;

        /* Fetch prime index for p */
        uint32_t idxp = prime_index[(uint32_t)p];

        /* Ensure spacing >= 3 with all previous prime indices */
        for (size_t i = 0; i < num_factors; ++i) {
            uint32_t diff = (idxp > indices[i]) ? (idxp - indices[i]) :
                                              (indices[i] - idxp);
            if (diff < 3) return false;
        }
        indices[num_factors++] = idxp;

        n /= p;                      /* remove one factor of p */
    }
    return true;
}

/* ---- Public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1, return 0 */

    /* Initialise global data on first use */
    if (!terms) {
        terms = seq_alloc_new_array(TERMS_CAP);
        build_sieve(200000);        /* generous upper bound for primes */
    }

    /* Generate terms up to the requested index */
    while (terms_len < (size_t)n) {
        /* current_candidate must be within the sieve */
        if (current_candidate > sieve_limit) abort();

        if (good_candidate(current_candidate)) {
            /* Store the qualifying number */
            terms[terms_len++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }

    return terms[n - 1];
}
