/*
 * A356862  Numbers with a unique largest prime exponent.
 * https://oeis.org/A356862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with a unique largest prime exponent (OEIS A356862).
   Formula: a(n) is the nth positive integer whose prime factorisation has a
   unique largest exponent. */

/* Utility: smallest‑prime‑factor sieve (provided by the framework). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"   /* void build_spf(idx_t limit, idx_t **spf_out); */

/* Static cache for the sequence values. */
static seq_t *seq_cache = NULL;   /* array of terms */
static size_t seq_cap  = 0;       /* current capacity of seq_cache */
static size_t seq_len  = 0;       /* number of terms stored */

/* Static smallest‑prime‑factor table. */
static idx_t *spf      = NULL;   /* spf[i] = smallest prime divisor of i */
static idx_t spf_limit = 0;       /* maximum index in spf that is valid */

/* Largest integer that has been examined so far when generating terms. */
static idx_t max_checked = 0;

/* ------------------------------------------------------------------ */
/* Grow the term cache when necessary. */
static void grow_seq_cache(void)
{
    size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure that the spf table covers numbers up to `new_limit`. */
static void ensure_spf(idx_t new_limit)
{
    if (new_limit <= spf_limit) return;
    if (spf) free(spf);
    build_spf(new_limit, &spf);
    spf_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Extend the sequence until it contains at least `target` terms. */
static void ensure_terms(idx_t target)
{
    if (seq_len >= (size_t)target) return;   /* already enough terms */

    /* Ensure spf is ready for factorising the numbers we will examine. */
    if (spf_limit < 2) ensure_spf(2);

    /* Start searching from the first number not yet checked. */
    idx_t start = max_checked + 1;
    /* Heuristic chunk size that grows as we go. */
    idx_t chunk = 1000;

    while (seq_len < (size_t)target)
    {
        /* Determine the range to evaluate in this iteration. */
        idx_t limit = start + chunk - 1;
        if (limit > spf_limit) ensure_spf(limit);

        for (idx_t i = start; i <= limit; ++i)
        {
            idx_t x          = i;
            idx_t cur_max_exp = 0;   /* largest exponent found so far */
            size_t count_max  = 0;   /* how many primes attain that exponent */

            /* Factorise i using the spf table. */
            while (x > 1)
            {
                idx_t p = spf[x];
                idx_t e = 0;
                while (x % p == 0)
                {
                    ++e;
                    x /= p;
                }

                if (e > cur_max_exp)
                {
                    cur_max_exp = e;
                    count_max   = 1;
                }
                else if (e == cur_max_exp)
                {
                    ++count_max;
                }
            }

            /* If the largest exponent is unique, record the integer. */
            if (count_max == 1)
            {
                if (seq_len == seq_cap) grow_seq_cache();
                seq_cache[seq_len++] = (seq_t)i;
            }
        }

        /* Prepare for the next chunk. */
        max_checked = limit;
        start       = limit + 1;
        chunk      *= 2;
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point: return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, avoid UB */
    ensure_terms(n);
    return seq_cache[(size_t)n - 1];
}
