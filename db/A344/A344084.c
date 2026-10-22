/*
 * A344084  Concatenated list of all finite nonempty sets of positive integers
 *          sorted first by maximum, then by length, and finally
 *          lexicographically.
 * https://oeis.org/A344084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th element of the concatenated list of all finite
 *            nonempty sets of positive integers sorted first by maximum, then
 *            by set size, and finally lexicographically. The implementation
 *            finds the set containing the n-th element and returns the
 *            appropriate element
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */
#include <stddef.h>          /* for size_t */
#include <stdbool.h>         /* for bool */
#include "../utils/binom.h"  /* seq_t binom(idx_t a, idx_t b) */

/* helper: convert seq_t (arbitrary‑precision integer) to idx_t.
   The values we use here are guaranteed to fit in idx_t. */
static inline idx_t to_idx(seq_t x) { return (idx_t)x; }

/* Unrank the lexicographic combination of size r from the set {1..n}.
   The elements are written into out (must be of length at least r). */
static void unrank_comb(idx_t n, idx_t r, idx_t rank, idx_t *out)
{
    idx_t pos = 0;
    idx_t start = 1;
    while (r > 0) {
        idx_t cnt = to_idx(binom(n - start, r - 1));
        if (rank < cnt) {
            out[pos++] = start;
            r--;
        } else {
            rank -= cnt;
        }
        start++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1, guard */
    /* Find the maximum m whose sets contribute at least n elements. */
    idx_t m = 1;
    idx_t cum = 0;
    while (1) {
        idx_t elem_count;                /* elements contributed by sets with max m */
        if (m == 1) elem_count = 1;
        else elem_count = (idx_t)(m + 1) << (m - 2);  /* (m+1)*2^(m-2) */
        if (cum + elem_count >= n) break;
        cum += elem_count;
        ++m;
    }

    /* Position of the desired element within the blocks of maximum m */
    idx_t offset = n - cum - 1;          /* 0‑based offset in this group */

    /* Determine size k of the set containing the element */
    idx_t k = 1;
    idx_t found = 0;
    for (; k <= m; ++k) {
        idx_t num_sets = to_idx(binom(m - 1, k - 1));
        idx_t block_size = k * num_sets;
        if (offset < block_size) {
            found = 1;
            break;
        }
        offset -= block_size;
    }

    /* Within the chosen size group */
    idx_t set_index  = offset / k;       /* index of the set among size‑k sets */
    idx_t elem_index = offset % k;       /* index of the element in the set */

    /* The first (k-1) elements are a combinational subset of {1..m-1},
       the last element is m. */
    if (elem_index == k - 1) {           /* last element of the set */
        return (seq_t)m;
    } else {
        /* get the combination (k-1)-subset for rank set_index */
        idx_t comb[64];                 /* m <= 63 for all tested values */
        unrank_comb(m - 1, k - 1, set_index, comb);
        return (seq_t)comb[elem_index];
    }
}
