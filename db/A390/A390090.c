/*
 * A390090  Irregular triangle read by rows where row n lists the first
 *          differences of the binary indices of n.
 * https://oeis.org/A390090
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by flattening all first differences of the
 *            binary indices of n (indices of 1‑bits counted from LSB = 1),
 *            starting from n = 3. The sequence is the triangle read by rows;
 *            rows that contain no differences contribute nothing. The offset
 *            is 3, so a(n) = diffs[n‑3] where diffs is the c...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> diffs;   /* flattened differences */
#else
static seq_t *diffs       = NULL; /* flattened differences */
static size_t diffs_len   = 0;     /* number of stored differences   */
static size_t diffs_cap   = 0;     /* allocated capacity             */
#endif

static idx_t next_row = 3;  /* next n to process */

/* ---------- C helper ---------------------------------------------------- */
#ifndef __cplusplus
static void grow_if_needed(size_t needed)
{
    if (needed <= diffs_cap) return;
    size_t new_cap = diffs_cap ? diffs_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(diffs, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    diffs = tmp;
    diffs_cap = new_cap;
}

static void add_diff(seq_t d)
{
    grow_if_needed(diffs_len + 1);
    diffs[diffs_len++] = d;
}
#endif
/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
#define ADD_DIFF(d) diffs.push_back(d)
#else
#define ADD_DIFF(d) add_diff(d)
#endif

/* Compute the first differences of the binary indices of n */
static void compute_row(idx_t n)
{
    idx_t bit   = 1;        /* current bit position (1‑based) */
    idx_t prev  = 0;        /* previous 1‑bit position */
    bool first = true;      /* flag for the first 1‑bit */
    idx_t temp  = n;

    while (temp)
    {
        if (temp & 1ULL)
        {
            if (!first)
            {
                seq_t diff = (seq_t)(bit - prev);
                ADD_DIFF(diff);
            }
            prev  = bit;
            first = false;
        }
        temp >>= 1;
        ++bit;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0; /* offset does not matter below 3 */

    size_t needed = (size_t)(n - 2);        /* number of differences required */
#ifdef __cplusplus
    while (diffs.size() < needed)
#else
    while (diffs_len < needed)
#endif
    {
        compute_row(next_row++);
    }

#ifdef __cplusplus
    return diffs[n - 3];
#else
    return diffs[n - 3];
#endif
}
