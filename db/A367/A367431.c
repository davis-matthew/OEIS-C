/*
 * A367431  Write down the positive integers. To obtain the terms of the
 *          sequence, concatenate groups of these so that the last number of
 *          each concatenated group is a prime.
 * https://oeis.org/A367431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of consecutive integers in the interval *
 *            [p_{n-1}+1 , p_n] where p_n is the n‑th prime (p_0=0). * The
 *            terms are produced in order: 12, 3, 45, 67, 891011, … * The
 *            function builds the sequence incrementally and memoises * all
 *            previously computed values
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 9, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_count, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Utility includes ----- */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */
#include "../utils/digit_count.h" /* uint64_t digit_count(uint64_t n); */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e); */

/* ----- Cached term array ----- */
static seq_t *term_cache   = NULL; /* array of computed terms */
static size_t term_cap    = 0;    /* allocated capacity */
static size_t term_cnt    = 0;    /* number of terms currently stored */

/* ----- State for incremental generation ----- */
static idx_t next_start = 1; /* next integer to start a new group */

/* Ensure term_cache has at least need elements allocated. */
static void ensure_terms_capacity(size_t need)
{
    if (need <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);      /* out of memory */
    term_cache = tmp;
    term_cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1 – return 0 for safety */
        return 0;
    /* 1‑based to 0‑based index conversion */
    size_t idx = (size_t)n;

    /* Compute terms up to n if not already cached. */
    if (idx > term_cnt) {
        ensure_terms_capacity(idx);

        for (size_t g = term_cnt; g < idx; ++g) {
            /* Determine group boundaries: [start .. end] where end is prime */
            idx_t start = next_start;
            idx_t end   = start;
            while (!is_prime(end))
                ++end;                /* find the next prime */

            /* Concatenate numbers in this group. */
            seq_t val = 0;
            for (idx_t num = start; num <= end; ++num) {
                /* number of decimal digits of num */
                uint64_t d = digit_count((uint64_t)num);
                seq_t pow = pow10((idx_t)d);
                val = val * pow + (seq_t)num;
            }

            term_cache[g] = val;
            next_start    = end + 1;            /* start of next group */
        }
        term_cnt = idx;
    }

    return term_cache[n - 1];
}
