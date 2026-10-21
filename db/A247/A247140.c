/*
 * A247140  Number of distinct values obtained from 1 to n when listing all
 *          distinct values of both numerators and denominators of sigma(n)/n.
 * https://oeis.org/A247140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247140
   Formula: a(n) = |{x : x appears as numerator or denominator of sigma(k)/k in lowest terms for 1 ≤ k ≤ n}|
   The function memoises values up to the maximum n requested so far.
   It recomputes all data from scratch whenever a larger n is queried.
   Complexity: O(n^2) time and O(n) memory for the largest n seen.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static idx_t   current_max = 0;          /* maximum n processed so far   */
static seq_t *a_arr        = NULL;       /* array of a(n) values          */
static seq_t *sigma_arr    = NULL;       /* array of sigma(k) values      */
static seq_t *seen_vals    = NULL;       /* array of distinct integers    */
static size_t  seen_cap     = 0;          /* capacity of seen_vals          */
static size_t  seen_cnt     = 0;          /* number of distinct integers   */

/* Recompute all data up to new_max (inclusive). */
static void recompute_until(idx_t new_max)
{
    /* free previous memory */
    if (a_arr)     free(a_arr);
    if (sigma_arr) free(sigma_arr);
    if (seen_vals) free(seen_vals);

    /* allocate sigma array */
    sigma_arr = (seq_t*)calloc((size_t)new_max + 1, sizeof(seq_t));
    if (!sigma_arr) { abort(); }

    /* compute sigma(k) via a simple divisor sieve */
    for (idx_t i = 1; i <= new_max; ++i) {
        for (idx_t j = i; j <= new_max; j += i) {
            sigma_arr[j] += (seq_t)i;
        }
    }

    /* allocate a_arr */
    a_arr = (seq_t*)calloc((size_t)new_max + 1, sizeof(seq_t));
    if (!a_arr) { abort(); }

    /* initialise storage for distinct numerators/denominators */
    seen_cap = (size_t)new_max * 2 + 10;
    seen_vals = (seq_t*)malloc(seen_cap * sizeof(seq_t));
    if (!seen_vals) { abort(); }
    seen_cnt = 0;

    /* iterate over k from 1 to new_max, building a(n) */
    for (idx_t k = 1; k <= new_max; ++k) {
        seq_t sigma_k = sigma_arr[k];
        seq_t g = gcd((seq_t)k, sigma_k);
        seq_t num = sigma_k / g;
        seq_t den = ((seq_t)k) / g;

        /* insert numerator if new */
        bool found = false;
        for (size_t i = 0; i < seen_cnt; ++i) {
            if (seen_vals[i] == num) { found = true; break; }
        }
        if (!found) {
            if (seen_cnt == seen_cap) {
                seen_cap *= 2;
                seen_vals = (seq_t*)realloc(seen_vals, seen_cap * sizeof(seq_t));
                if (!seen_vals) { abort(); }
            }
            seen_vals[seen_cnt++] = num;
        }

        /* insert denominator if new */
        found = false;
        for (size_t i = 0; i < seen_cnt; ++i) {
            if (seen_vals[i] == den) { found = true; break; }
        }
        if (!found) {
            if (seen_cnt == seen_cap) {
                seen_cap *= 2;
                seen_vals = (seq_t*)realloc(seen_vals, seen_cap * sizeof(seq_t));
                if (!seen_vals) { abort(); }
            }
            seen_vals[seen_cnt++] = den;
        }

        a_arr[k] = (seq_t)seen_cnt;
    }

    current_max = new_max;
}

/* Public function defined by the problem specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe fallback */
    if (n > current_max) {
        recompute_until(n);
    }
    return a_arr[n];
}
