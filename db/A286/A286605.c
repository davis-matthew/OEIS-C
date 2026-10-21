/*
 * A286605  Restricted growth sequence computed for number of divisors, d(n)
 *          (A000005).
 * https://oeis.org/A286605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Restricted‑growth mapping of divisor counts:
   a(n) = label assigned to d(n) (A000005) in order of first appearance. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is offset 1 */

    /* static cached data */
    static idx_t   max_n   = 0;
    static idx_t  *d_counts = NULL;      /* divisor counts for 1..max_n */
    static seq_t  *labels   = NULL;      /* a(i) values for 1..max_n */
    static int   *label_map = NULL;      /* mapping divisor count -> label */
    static size_t label_map_size = 0;
    static idx_t  seen_labels = 0;

    if (n > max_n) {
        /* resize arrays for new maximum index */
        idx_t i, j;
        idx_t new_max = n;

        d_counts = (idx_t *)realloc(d_counts, (new_max + 1) * sizeof(idx_t));
        labels   = (seq_t *)realloc(labels,   (new_max + 1) * sizeof(seq_t));
        if (!d_counts || !labels) abort(); /* allocation failure */

        /* initialise divisor counts */
        for (i = 1; i <= new_max; ++i) d_counts[i] = 0;
        for (i = 1; i <= new_max; ++i) {
            for (j = i; j <= new_max; j += i) ++d_counts[j];
        }

        /* find maximum divisor count */
        idx_t max_d = 0;
        for (i = 1; i <= new_max; ++i) {
            if (d_counts[i] > max_d) max_d = d_counts[i];
        }

        /* rebuild mapping table */
        free(label_map);
        label_map_size = (size_t)max_d + 1;
        label_map = (int *)calloc(label_map_size, sizeof(int));
        if (!label_map) abort();

        seen_labels = 0;
        for (i = 1; i <= new_max; ++i) {
            idx_t d = d_counts[i];
            int label = label_map[(size_t)d];
            if (label == 0) {
                ++seen_labels;
                label = (int)seen_labels;
                label_map[(size_t)d] = label;
            }
            labels[i] = (seq_t)label;
        }

        max_n = new_max;
    }

    return labels[n];
}
