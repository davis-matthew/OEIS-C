/*
 * A340152  Numbers k such that k and k+1 are both cubefree numbers (A004709).
 * https://oeis.org/A340152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer k such that k and k+1 are both cubefree (OEIS A340152). */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of found terms.  seq_list keeps the list of all terms
   found so far, seq_size is the number of cached terms, seq_cap the
   allocated capacity, and last_checked is the largest k processed
   (inclusive).  All indices are 1‑based as in the OEIS convention. */
static seq_t *seq_list   = NULL;
static size_t seq_size   = 0;
static size_t seq_cap    = 0;
static idx_t  last_checked = 0;

static void seq_append(seq_t val)
{
    if (seq_size == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 8 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc((void *)seq_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        seq_list = tmp;
        seq_cap  = new_cap;
    }
    seq_list[seq_size++] = val;
}

/* Returns true if m is cubefree (no prime cube divides it).  The loop
   runs only up to ∛m, which is very small for the range of numbers we
   need to examine in a test of a few thousands.  The operations are
   valid for built-in integer types and for the arbitrary‑precision
   classes used by the build system (operator* and operator% are
   defined). */
static bool is_cubefree(seq_t m)
{
    for (seq_t j = (seq_t)2;; ++j) {
        seq_t j3 = j * j * j;
        if (j3 > m) break;
        if (m % j3 == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 for safety */

    /* Generate terms up to the requested index if necessary. */
    while (seq_size < (size_t)n) {
        ++last_checked;
        seq_t k  = last_checked;
        seq_t k1 = k + (seq_t)1;
        if (is_cubefree(k) && is_cubefree(k1))
            seq_append(k);
    }
    return seq_list[(size_t)n - 1];
}
