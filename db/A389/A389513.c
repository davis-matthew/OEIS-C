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
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
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

/*  Test whether the composition corresponding to index k satisfies
 *  the maximal run condition.  The mapping from k to the composition
 *  follows the specification of A066099:
 *
 *  1.  Write k in binary, read from least‑significant bit first
 *      (i.e. standard bit order).  For each bit that is 1, record
 *      its position (starting at 1).
 *  2.  Prepend a 0 to this list of positions.
 *  3.  The parts of the composition are the successive differences
 *      between consecutive positions; no further reversal is needed.
 *  4.  Check each maximal run of equal parts: run length >= part value.
 */
static bool passes_condition(idx_t k)
{
    if (k == 0) return true;          /* empty composition */

    /* pos[0] = 0, pos[1..m] are positions of 1-bits (1‑based) */
    idx_t pos[65];   /* maximum 64 set bits + leading 0 */
    size_t m = 0;
    pos[0] = 0;

    idx_t t = k;
    idx_t bitpos = 1;                   /* bit positions start at 1 */
    while (t)
    {
        if (t & 1ULL) pos[++m] = bitpos;
        t >>= 1;
        bitpos++;
    }

    /* Run length check */
    idx_t cur = pos[1] - pos[0];        /* first part */
    size_t run_len = 1;

    for (size_t i = 2; i <= m; ++i)
    {
        idx_t part = pos[i] - pos[i-1];
        if (part == cur)
        {
            run_len++;
        }
        else
        {
            if (run_len < cur) return false;
            cur = part;
            run_len = 1;
        }
    }
    return run_len >= cur;
}

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
