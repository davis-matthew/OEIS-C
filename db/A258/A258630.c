/*
 * A258630  Number of length n 1..(7+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5 or 7.
 * https://oeis.org/A258630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays of integers 1..8 such that *
 *            every prefix sum is divisible by at least one of {2,3,5,7}. *
 *            The program uses dynamic programming over the residue modulo
 *            lcm(2,3,5,7)=210
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- dependencies --------------------------------------------------------- */
#include <stddef.h>
#include <stdbool.h>

/* The required utility functions for handling the generic seq_t type. */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* --- static state for memoisation ---------------------------------------- */
static seq_t *answers = NULL;   /* a(k) for k <= cache_n */
static size_t answers_cap = 0;
static idx_t cache_n = 0;       /* largest n computed so far */

static seq_t *dp = NULL;        /* distribution over residues modulo 210   */
static seq_t *next_arr = NULL;  /* temporary array for the next step      */

#define STATE_COUNT 210

/* --- public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* For index 0 the sequence is undefined; return 0 to keep behaviour sane. */
        return 0;
    }

    /* Ensure answers array can hold indices up to n. */
    if ((size_t)n + 1 > answers_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = seq_realloc(answers, answers_cap, new_cap);
        if (!tmp) return 0;               /* allocation failure */
        answers = tmp;
        answers_cap = new_cap;
        /* initialise the newly added elements to zero */
        for (size_t i = 0; i < new_cap; ++i) {
            answers[i] = 0;
        }
    }

    /* Return cached value if already computed. */
    if (n <= cache_n) {
        return answers[n];
    }

    /* First‑time initialisation of DP arrays. */
    if (dp == NULL) {
        dp = seq_alloc_new_array(STATE_COUNT);
        next_arr = seq_alloc_new_array(STATE_COUNT);
        if (!dp || !next_arr) {
            return 0;                    /* allocation failure */
        }
        /* zero initialise the arrays */
        for (size_t i = 0; i < STATE_COUNT; ++i) {
            dp[i] = 0;
            next_arr[i] = 0;
        }
        dp[0] = 1;                       /* empty sum */
        cache_n = 0;
    }

    /* Extend cache up to n. */
    for (idx_t step = cache_n + 1; step <= n; ++step) {
        /* Clear next_arr for the new length. */
        for (size_t i = 0; i < STATE_COUNT; ++i) {
            next_arr[i] = 0;
        }

        /* Transition from current dp to next_arr. */
        for (size_t r = 0; r < STATE_COUNT; ++r) {
            seq_t ways = dp[r];
            if (ways == 0) continue;
            for (int v = 1; v <= 8; ++v) {
                size_t newr = (r + v) % STATE_COUNT;
                /* Check divisibility condition for the new partial sum. */
                if ((newr & 1) == 0 ||
                    newr % 3 == 0 ||
                    newr % 5 == 0 ||
                    newr % 7 == 0) {
                    next_arr[newr] += ways;
                }
            }
        }

        /* Sum over all acceptable residues to obtain a(step). */
        seq_t sum = 0;
        for (size_t r = 0; r < STATE_COUNT; ++r) {
            sum += next_arr[r];
        }
        answers[step] = sum;

        /* Prepare for next iteration: dp becomes the new distribution. */
        seq_t *tmp_ptr = dp;
        dp = next_arr;
        next_arr = tmp_ptr;
    }

    cache_n = n;
    return answers[n];
}
