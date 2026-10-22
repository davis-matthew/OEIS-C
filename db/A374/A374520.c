/*
 * A374520  Numbers k such that the leaders of maximal anti-runs in the k-th
 *          composition in standard order (A066099) are not identical.
 * https://oeis.org/A374520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   leaders_not_identical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer k (starting at k = 0) for which the leaders
 * of maximal anti‑runs in the k-th composition in standard order
 * (graded reverse‑lexicographic, OEIS A066099) are not identical.
 * The implementation scans k sequentially, testing the property
 * and memoising results so that calls may appear in any order. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/leaders_not_identical.h"

/* Produce the k‑th composition in standard order.
 * 'parts' is an array that must be at least 64 elements long.
 * 'len' receives the number of parts (0 for k = 0). */
static void comp_from_k(idx_t k, seq_t *parts, size_t *len)
{
    if (k == 0) {
        *len = 0;
        return;
    }

    /* Positions of set bits in the reversed binary representation
     * (i.e., bits of k collected from LSB up to MSB), counted starting at 1. */
    seq_t poslist[64];
    size_t m = 0;         /* number of set bits */
    idx_t tmp = k;
    size_t pos = 0;       /* LSB position, starting at 0 */

    while (tmp) {
        if (tmp & 1ULL) {
            poslist[m++] = (seq_t)(pos + 1);
        }
        pos++;
        tmp >>= 1;
    }

    /* Build list = [0, poslist...] */
    seq_t list[65];
    list[0] = 0;
    for (size_t i = 0; i < m; ++i)
        list[i + 1] = poslist[i];

    /* Compute first differences: parts[i] = list[i+1] - list[i] */
    for (size_t i = 0; i < m; ++i)
        parts[i] = list[i + 1] - list[i];
    *len = m;

    /* Reverse the parts array to obtain the standard order composition. */
    for (size_t i = 0, j = m - 1; i < j; ++i, --j) {
        seq_t tmp = parts[i];
        parts[i] = parts[j];
        parts[j] = tmp;
    }
}

/* Memoised sequence a(n). */
seq_t a(idx_t n)
{
    static seq_t *found      = NULL;   /* array of k values */
    static size_t found_size = 0;
    static size_t found_cap  = 0;
    static idx_t next_k      = 0;       /* next k to test */

    /* Ensure we have computed enough terms. */
    while (found_size < (size_t)n) {
        /* Get composition for current k. */
        seq_t parts[64];
        size_t len = 0;
        comp_from_k(next_k, parts, &len);

        /* Test the property. */
        if (len > 0 && leaders_not_identical(parts, len)) {
            /* Store k as seq_t. */
            if (found_size + 1 > found_cap) {
                size_t new_cap = (found_cap == 0) ? 16 : found_cap * 2;
                seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                found = tmp;
                found_cap = new_cap;
            }
            found[found_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return found[n - 1];
}
