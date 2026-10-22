/*
 * A389513  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) >= first(R).
 * https://oeis.org/A389513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   passes_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = the n‑th index (0‑based) of a composition in standard
 *  (graded reverse‑lexicographic, A066099) order that satisfies the
 *  maximal run condition: for every maximal run R of equal parts,
 *  length(R) >= first(R).
 *
 *  The implementation searches sequentially for qualifying compositions
 *  and caches the indices in an array so that multiple calls are
 *  efficient.  Only standard C11 operations are used.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/passes_condition.h"

/* Forward declaration of the test helper */
static bool passes_condition(idx_t k);

/* Cached qualifying indices */
static idx_t *qual_fwd = NULL;   /* array of qualifying indices */
static size_t qual_cnt = 0;      /* number of stored indices      */
static size_t qual_cap = 0;      /* capacity of qual_fwd          */
static idx_t next_test_k = 0;    /* next composition index to test */

/* Ensure the cache has at least 'need' slots */
static void grow_cache(size_t need)
{
    if (need > qual_cap)
    {
        size_t new_cap = qual_cap ? qual_cap * 2 : 8;
        while (new_cap < need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(qual_fwd, new_cap * sizeof(idx_t));
        qual_fwd = tmp;
        qual_cap = new_cap;
    }
}

/*  Public function as required by the problem description.  */
seq_t a(idx_t n)
{
    /* n is 1‑based; return the n‑th qualifying index (0‑based) */
    if (n == 0) return 0;          /* not used but harmless */

    /* Ensure the cache contains at least n entries */
    if (qual_cnt < (size_t)n)
    {
        grow_cache(n);

        while (qual_cnt < (size_t)n)
        {
            if (passes_condition(next_test_k))
            {
                qual_fwd[qual_cnt++] = next_test_k;
            }
            next_test_k++;
        }
    }

    return (seq_t)qual_fwd[n-1];
}
