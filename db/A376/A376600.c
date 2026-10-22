/*
 * A376600  Inflection or undulation points in the sequence of
 *          non-prime-powers inclusive (A024619).
 * https://oeis.org/A376600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inflection points of the sequence of non‑prime‑powers inclusive (A024619).
   a(n) is the n‑th index i such that the first differences of A024619
   are equal: diff1(i) == diff1(i+1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

/* Test whether x is a prime power (including 1). */
static inline bool is_prime_power(uint64_t x)
{
    if (x <= 1) return true;            /* 1 is deemed a prime power */
    return count_distinct_primes(x) == 1;
}

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static bool initialized = false;
    static idx_t *indices = NULL;   /* indices where diff1(i) == diff1(i+1)   */
    static size_t cap = 0;          /* allocated capacity for indices      */
    static size_t sz = 0;           /* number of stored indices            */

    static seq_t prev_val;          /* A024619[k-1] as seq_t              */
    static seq_t curr_val;          /* A024619[k]   as seq_t              */
    static seq_t diff_prev;         /* diff1(k-1) = A[k] - A[k-1]         */
    static idx_t curr_k;            /* current index k of curr_val (1‑based) */
    static uint64_t next_candidate; /* next integer to test for non‑prime‑power */

    /* Initialise on first call */
    if (!initialized) {
        initialized = true;
        prev_val = (seq_t)6;                    /* first term of A024619 */
        curr_val = (seq_t)10;                   /* second term */
        curr_k   = 2;                           /* index of curr_val */
        diff_prev = curr_val - prev_val;        /* diff1(1) */
        next_candidate = 11;                    /* start searching beyond 10 */
    }

    /* Generate enough indices to answer n */
    while (sz < n) {
        /* Find next non‑prime‑power after curr_val */
        uint64_t cand = next_candidate;
        while (is_prime_power(cand)) ++cand;
        seq_t new_val = (seq_t)cand;

        seq_t diff_current = new_val - curr_val;   /* diff1(curr_k) */

        /* If consecutive differences are equal, record index */
        if (diff_current == diff_prev) {
            /* Index i = curr_k - 1 (1‑based) */
            if (sz == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                idx_t *tmp = (idx_t *)realloc(indices, new_cap * sizeof(idx_t));
                if (!tmp) abort();                 /* allocation failure */
                indices = tmp;
                cap = new_cap;
            }
            indices[sz++] = curr_k - 1;
        }

        /* Advance to next A024619 term */
        diff_prev = diff_current;
        prev_val = curr_val;
        curr_val = new_val;
        ++curr_k;
        next_candidate = (uint64_t)curr_val + 1;
    }

    return (seq_t)indices[n - 1];
}
