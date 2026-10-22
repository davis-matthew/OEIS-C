/*
 * A351720  Numbers k such that k and k + 1 are both lazy-Lucas-Niven numbers
 *          (A351719).
 * https://oeis.org/A351720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A351720
   Find all k such that k and k+1 are both lazy‑Lucas‑Niven numbers
   (numbers divisible by the number of 1‑digits in their maximal
   Lucas representation).  The function pre‑computes enough terms
   up to a fixed upper bound (2×10⁷) and returns the n‑th term.
   It uses a 0/1 knapsack over Lucas numbers to obtain the maximal
   number of 1‑digits for every integer up to the bound.  The
   implementation is iterative and keeps its lookup table static so
   successive calls are fast.  It works for any seq_t/idx_t pair
   provided by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static helper for dynamic array reallocation */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Main OEIS function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* --- static state ------------------------------------------ */
    static bool initialized = false;
    static size_t max_limit = 0;          /* upper bound of sieve      */
    static int16_t *dp          = NULL;   /* maximal 1‑cnt array       */
    static seq_t  *term_list    = NULL;   /* collected sequence terms   */
    static size_t term_len      = 0;       /* number of terms stored     */
    static size_t term_cap      = 0;       /* capacity of term_list      */

    if (n == 0) return (seq_t)0;          /* offset is 1, so 0 is invalid */

    /* --- initialise tables once --------------------------------- */
    if (!initialized) {
        const size_t LIMIT = 20000000U;            /* fixed upper bound      */
        max_limit = LIMIT;

        /* allocate dp array and set unreachable to -1 */
        dp = (int16_t *)calloc(max_limit + 1, sizeof(int16_t));
        if (!dp) return (seq_t)0;                     /* allocation failure */
        dp[0] = 0;
        for (size_t i = 1; i <= max_limit; ++i) dp[i] = -1;

        /* generate Lucas numbers up to LIMIT */
        uint64_t luc[64];
        size_t lc = 0;
        luc[lc++] = 2ULL;          /* L0 */
        luc[lc++] = 1ULL;          /* L1 */
        while (true) {
            uint64_t nxt = luc[lc - 1] + luc[lc - 2];
            if (nxt > (uint64_t)max_limit) break;
            luc[lc++] = nxt;
        }

        /* 0/1 knapsack maximise number of 1‑digits */
        for (size_t i = 0; i < lc; ++i) {
            size_t val = (size_t)luc[i];
            for (size_t s = max_limit; s >= val; --s) {
                if (dp[s - val] >= 0) {
                    int16_t cand = dp[s - val] + 1;
                    if (cand > dp[s]) dp[s] = cand;
                }
            }
        }

        /* allocate dynamic sequence array */
        term_cap = 128;
        term_list = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!term_list) { free(dp); return (seq_t)0; }

        /* collect all k such that k and k+1 are lazy‑Lucas‑Niven */
        for (size_t k = 1; k + 1 <= max_limit; ++k) {
            int16_t c1 = dp[k];
            int16_t c2 = dp[k + 1];
            if (c1 >= 0 && c2 >= 0) {
                seq_t val_k  = (seq_t)k;
                seq_t val_k1 = (seq_t)(k + 1);
                if (val_k  % (seq_t)c1 == 0 &&
                    val_k1 % (seq_t)c2 == 0) {
                    /* store the term */
                    if (term_len == term_cap) {
                        grow_seq_array(&term_list, &term_cap, term_len + 1);
                        if (!term_list) { free(dp); return (seq_t)0; }
                    }
                    term_list[term_len++] = val_k;
                }
            }
        }

        initialized = true;
    }

    /* out of range? */
    if ((idx_t)term_len < n) return (seq_t)0;
    return term_list[(size_t)n - 1];
}
