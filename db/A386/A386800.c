/*
 * A386800  Numbers that have exactly one exponent in their prime
 *          factorization that is equal to 3.
 * https://oeis.org/A386800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 6081, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386800: Numbers with exactly one exponent equal to 3 in their prime factorization.
   Computed by sequential search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_realloc.h"   /* for seq_realloc */

/* static caches for the sequence */
static seq_t *term_arr   = NULL;   /* array of sequence terms */
static size_t term_len   = 0;      /* number of terms stored */
static size_t term_cap   = 0;      /* allocated capacity */

/* smallest‑prime‑factor table */
static uint64_t *spf      = NULL;  /* spf[i] = smallest prime factor of i */
static idx_t     spf_limit = 0;    /* highest index currently valid */

/* last integer examined for sequence membership */
static idx_t last_checked = 1;

/* ------------------------------------------------------------ */
/* Build prime‑factor sieve up to n (inclusive).  Expands the
   existing table if necessary. */
static void build_spf(idx_t n)
{
    if (spf_limit >= n) return;      /* already large enough */

    /* allocate or enlarge the spf array */
    uint64_t *new_spf = (uint64_t *)realloc(spf, (n + 1) * sizeof(uint64_t));
    if (!new_spf) {
        /* out of memory – abort */
        abort();
    }
    spf = new_spf;

    /* initialise new entries */
    for (idx_t i = spf_limit + 1; i <= n; ++i) spf[i] = i;

    /* sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_limit = n;
}

/* Count how many prime exponents in the factorisation of v are equal to 3. */
static int count_exponent3(uint64_t v)
{
    int cnt = 0;
    while (v > 1) {
        uint64_t p = spf[v];
        uint64_t e = 0;
        while (v % p == 0) {
            v /= p;
            ++e;
        }
        if (e == 3) ++cnt;
    }
    return cnt;
}

/* Ensure that the first n terms of the sequence are stored. */
static void ensure_terms(idx_t n)
{
    if (term_len >= n) return;          /* already have enough */

    /* grow the term array if needed */
    if (term_cap < n) {
        size_t new_cap = term_cap ? term_cap : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(term_arr, term_cap, new_cap);
        if (!tmp) abort();
        term_arr = tmp;
        term_cap = new_cap;
    }

    /* scan integers one by one until enough terms found */
    for (uint64_t x = last_checked + 1;
         term_len < n;
         ++x, ++last_checked)
    {
        /* ensure spf covers x */
        if (x > spf_limit) build_spf(x * 2);

        if (count_exponent3(x) == 1) {
            term_arr[term_len++] = (seq_t)x;
        }
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n==0 (offset 1) */
    ensure_terms(n);
    return term_arr[n - 1];
}
