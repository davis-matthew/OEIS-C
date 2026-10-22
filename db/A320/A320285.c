/*
 * A320285  Semiprimes followed by successive gaps 4, 6, 9.
 * https://oeis.org/A320285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A320285: semiprimes followed by successive gaps 4, 6, 9.
 * The function searches for numbers s such that
 *   s+4, s+10 and s+19 are also semiprimes, and no other semiprime
 *   occurs in the gaps (1,2,3), (5,6,7,8,9) or (11,…,18).
 * It returns the n‑th such s (1‑based index). The routine keeps a
 * global sieve of smallest prime factors and continues searching
 * incrementally between calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global data ------------------------------------------- */
static uint32_t *spf          = NULL;   /* smallest‑prime‑factor table */
static idx_t    spf_limit     = 0;      /* current table limit          */
static idx_t    next_search   = 2;      /* next candidate to test      */
static idx_t    count_found   = 0;      /* number of terms found so far*/

/* Build a smallest–prime–factor array up to 'limit'. */
static uint32_t *build_spf(idx_t limit)
{
    uint32_t *arr = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!arr) exit(EXIT_FAILURE);

    for (idx_t i = 2; (uint64_t)i * i <= (uint64_t)limit; ++i) {
        if (arr[i] == 0) {
            uint32_t p = (uint32_t)i;
            for (idx_t j = i * (uint64_t)i; j <= limit; j += i)
                if (arr[j] == 0) arr[j] = p;
        }
    }
    for (idx_t i = 2; i <= limit; ++i)
        if (arr[i] == 0) arr[i] = (uint32_t)i;
    return arr;
}

/* Ensure we have a sieve large enough to cover 'need'.  */
static void ensure_spf(idx_t need)
{
    if (spf_limit >= need) return;
    free(spf);
    spf = build_spf(need);
    spf_limit = need;
}

/* Test whether n is a semiprime (product of exactly two primes). */
static bool is_semiprime(idx_t n)
{
    if (n < 4 || n > spf_limit) return false;
    uint32_t p = spf[n];
    if (p == 0 || p == n) return false;          /* prime or 1 */
    idx_t q = n / p;
    return spf[q] == q;                          /* q must be prime */
}

/* Main function: return the n-th term. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                  /* defensive */

    while (count_found < n) {
        /* Make sure the sieve covers the current search range. */
        idx_t need = next_search + 19;
        if (spf_limit < need) {
            idx_t new_limit = spf_limit * 2;
            if (new_limit < need) new_limit = need;
            if (new_limit < 1000) new_limit = 1000;
            ensure_spf(new_limit);
        }

        /* Scan candidates. */
        for (idx_t m = next_search; m + 19 <= spf_limit; ++m) {
            if (!is_semiprime(m)) continue;

            /* Check the required semiprimes at the gaps. */
            if (!(is_semiprime(m + 4) && is_semiprime(m + 10) && is_semiprime(m + 19)))
                continue;

            /* Ensure no other semiprime in the forbidden slots. */
            bool ok = true;
            for (idx_t d = 1; d <= 3 && ok; ++d)
                if (is_semiprime(m + d)) ok = false;
            for (idx_t d = 5; d <= 9 && ok; ++d)
                if (is_semiprime(m + d)) ok = false;
            for (idx_t d = 11; d <= 18 && ok; ++d)
                if (is_semiprime(m + d)) ok = false;

            if (ok) {
                ++count_found;
                if (count_found == n) {
                    next_search = m + 1;
                    return (seq_t)m;
                }
                /* else store next search start to skip this block */
                next_search = m + 1;
            }
        }

        /* If we reach the end of current sieve, extend it. */
        idx_t new_limit = spf_limit * 2;
        if (new_limit < next_search + 19) new_limit = next_search + 19;
        ensure_spf(new_limit);
    }

    /* Should never reach here. */
    return (seq_t)0;
}
