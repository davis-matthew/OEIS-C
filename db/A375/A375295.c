/*
 * A375295  Numbers k such that the leaders of maximal weakly increasing runs
 *          in the k-th composition in standard order (row k of A066099) are
 *          not strictly decreasing.
 * https://oeis.org/A375295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375295: numbers k such that the leaders of maximal weakly
 * increasing runs in the k-th composition of A066099 are not strictly
 * decreasing.  The function generates the compositions in standard
 * order and collects those k that satisfy the property.  The first few
 * terms are 13, 25, 27, 29, 45, 49, 50, 51, 53, 54, … */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/property.h"

/* Recursive enumeration of all compositions of 'rem' in
 * descending lexicographic order.  'pos' is the next index in
 * 'parts' where the new part will be written.  The function
 * stops early once enough terms have been found. */
static void enumerate(int rem, int pos, idx_t *k, size_t *cnt,
                      size_t needed, seq_t *vals, int *parts)
{
    if (*cnt >= needed) return;          /* early exit */

    if (rem == 0) {                      /* complete composition */
        (*k)++;                           /* increment global index */
        if (*cnt < needed && property(parts, pos)) {
            vals[*cnt] = (seq_t)(*k);
            (*cnt)++;
        }
        return;
    }

    for (int part = rem; part >= 1; --part) {
        parts[pos] = part;
        enumerate(rem - part, pos + 1, k, cnt, needed, vals, parts);
        if (*cnt >= needed) return;       /* no more needed */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard */

    size_t needed = (size_t)n;
    seq_t *vals = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!vals) return (seq_t)0;           /* allocation failure guard */

    size_t cnt = 0;
    idx_t k = 0;                          /* zero before first composition */

    int sum = 1;
    int parts[64];                        /* max sum to be encountered < 64 */

    while (cnt < needed) {
        enumerate(sum, 0, &k, &cnt, needed, vals, parts);
        ++sum;
    }

    seq_t result = vals[needed - 1];
    free(vals);
    return result;
}
