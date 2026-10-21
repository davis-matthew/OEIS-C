/*
 * A276380  Irregular triangle where row n contains terms k of the partition
 *          of n produced by greedy algorithm such that all elements are in
 *          A003586.
 * https://oeis.org/A276380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 581, u16 n <= 9502, u32 n <= 9502, u64 n <= 9502,
 *            u128 n <= 9502, big any n
 * Helpers:   alloc_seq, largest_double_base_leq, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th element of the irregular triangle
   described in OEIS A276380.  For a given row r (r ≥ 1) the greedy
   algorithm builds a partition of r into terms that are of the form
   2^a·3^b (the A003586 numbers).  The terms of each row are sorted
   ascending; the rows are concatenated to form the sequence.
   The function uses a simple incremental construction and a dynamic
   array that grows by doubling when necessary.  The helper functions
   ``alloc_seq`` and ``seq_free`` are used to allocate memory that
   works for both integer and arbitrary‑precision seq_t types.  The
   algorithm runs in O(n) time and uses O(total elements) memory.   */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/* Use the allocator helpers that are available in the build environment
   (these are defined in the utils directory).  */
#include "../utils/alloc_seq.h"
#include "../utils/seq_free.h"
#include "../utils/largest_double_base_leq.h"

/* Append a single row to the global sequence array. */
static void build_row(idx_t row,
                      seq_t **seq, size_t *len, size_t *cap)
{
    idx_t rem = row;
    seq_t temp[64];
    size_t tlen = 0;

    /* Greedy partition of the row. */
    while (rem > 0) {
        seq_t d = largest_double_base_leq((idx_t)rem);
        temp[tlen++] = d;
        rem -= (idx_t)d;
    }

    /* Sort the temporary array ascending. */
    for (size_t i = 1; i < tlen; ++i) {
        seq_t key = temp[i];
        size_t j = i;
        while (j > 0 && temp[j-1] > key) {
            temp[j] = temp[j-1];
            --j;
        }
        temp[j] = key;
    }

    /* Grow the global array if necessary. */
    if (*len + tlen > *cap) {
        size_t newcap = (*cap) ? (*cap) * 2 : 8;
        while (newcap < *len + tlen) newcap <<= 1;

        seq_t *newp = alloc_seq(newcap);
        for (size_t i = 0; i < *len; ++i)
            newp[i] = (*seq)[i];
        if (*seq != NULL) seq_free(*seq);
        *seq = newp;
        *cap = newcap;
    }

    /* Append the sorted row. */
    for (size_t i = 0; i < tlen; ++i)
        (*seq)[*len + i] = temp[i];
    *len += tlen;
}

/* Global dynamic array holding the concatenated rows. */
static seq_t *global_seq = NULL;
static size_t global_len = 0;
static size_t global_cap = 0;
static idx_t next_row = 0;

/* Return the n-th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    while (global_len < (size_t)n) {
        build_row(next_row, &global_seq, &global_len, &global_cap);
        ++next_row;
    }
    return global_seq[n-1];
}
