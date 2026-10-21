/*
 * A275580  Add square root of sum of terms.
 * https://oeis.org/A275580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(0) = 1; a(n) = a(n‑1) + floor(sqrt( Sum_{i=0}^{n‑1} a(i) ))
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 51, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Memoised storage for the sequence and its prefix sums.
   seq  [i]   = a(i)
   pref [j]   = sum_{k=0}^{j-1} a(k)   (pref[0] = 0)   */
static seq_t *seq   = NULL;
static seq_t *pref  = NULL;
static size_t seq_cap = 0;          /* allocated size of seq  */
static size_t current_len = 0;      /* number of computed values */

/* Ensure that the arrays can hold at least (n+1) sequence elements. */
static void ensure_capacity(idx_t n)
{
    if (seq_cap >= (size_t)(n + 1))
        return;   /* already large enough */

    size_t old_cap = seq_cap;
    size_t new_cap = old_cap ? old_cap : 1;
    /* Grow until we have room for the requested index (+1 for pref). */
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq  = (seq_t *)realloc(seq,  new_cap * sizeof(seq_t));
    pref = (seq_t *)realloc(pref, (new_cap + 1) * sizeof(seq_t));

    /* Initialise the newly allocated part of pref.  Do NOT touch
       pref indices that already contain valid prefix sums. */
    for (size_t i = old_cap + 1; i <= new_cap; ++i)
        pref[i] = 0;

    seq_cap = new_cap;
}

/* Initialisation of the base case a(0) = 1. */
static void init_base(void)
{
    if (current_len != 0)
        return;               /* already initialised */

    ensure_capacity(0);        /* allocate the minimal array */
    seq[0] = 1;
    pref[0] = 0;
    pref[1] = 1;               /* pref[1] = a(0) */
    current_len = 1;
}

/* The required function. */
seq_t a(idx_t n)
{
    init_base();              /* make sure a(0) is ready */

    if (n >= current_len) {
        /* Grow arrays if necessary. */
        ensure_capacity(n);

        /* Compute terms up to n. */
        for (idx_t k = current_len; k <= n; ++k) {
            seq_t sum = pref[k];          /* sum_{i=0}^{k-1} a(i) */
            seq_t add = floor_sqrt(sum);  /* floor(sqrt(sum)) */
            seq_t val = seq[k-1] + add;   /* a(k) */
            seq[k] = val;
            pref[k+1] = sum + val;        /* new prefix sum */
        }
        current_len = n + 1;
    }

    return seq[n];
}
