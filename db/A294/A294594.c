/*
 * A294594  Numbers that are the sum of three squares (square 0 allowed) in
 *          exactly five ways.
 * https://oeis.org/A294594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer that can be expressed as a sum
 *            of three squares (0 allowed) in exactly five unordered ways
 *            (ignoring signs and order). The code builds the sequence lazily
 *            by enumerating all unordered triples up to a current bound and
 *            collecting those with count 5
 * Range:     u8 n <= 8, u16 n <= 772, u32 n <= 843, u64 n <= 843,
 *            u128 n <= 843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* Helper: for a given upper bound LIMIT, count how many unordered
   representations each integer up to LIMIT has as a sum of three squares.
   The count array may be reused; its elements are capped at 6 to avoid overflow. */
static void compute_counts(idx_t limit, uint8_t *counts)
{
    idx_t maxa = (idx_t)sqrt((double)limit);          /* floor sqrt(limit) */
    idx_t *sq = (idx_t *)malloc((maxa + 1) * sizeof(idx_t));
    if (!sq) return;                                 /* allocation failure */
    for (idx_t i = 0; i <= maxa; ++i) sq[i] = i * i;

    for (idx_t a = 0; a <= maxa; ++a) {
        idx_t sa = sq[a];
        for (idx_t b = 0; b <= a; ++b) {
            idx_t sab = sa + sq[b];
            if (sab > limit) break;
            for (idx_t c = 0; c <= b; ++c) {
                idx_t s = sab + sq[c];
                if (s > limit) break;
                if (counts[s] < 6) ++counts[s];
            }
        }
    }
    free(sq);
}

/* Returns the n‑th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *values = NULL;
    static size_t values_len = 0;
    static size_t values_cap = 0;
    static idx_t max_checked = 0;

    if (n == 0) return (seq_t)0;        /* undefined, but safe */

    /* Grow the array holding the found terms if necessary */
    if (values_cap < values_len + 1) {
        size_t new_cap = values_cap ? values_cap * 2 : 32;
        if (new_cap < values_len + 1) new_cap = values_len + 1;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        values = tmp;
        values_cap = new_cap;
    }

    /* Generate more terms until we have at least n of them */
    if (values_len < n) {
        idx_t limit = max_checked ? max_checked * 2 : 20000; /* initial bound */

        while (values_len < n) {
            /* Allocate count array for current limit */
            uint8_t *counts = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
            if (!counts) return (seq_t)0;        /* allocation failure */

            compute_counts(limit, counts);

            /* Collect qualifying numbers not seen before */
            for (idx_t x = max_checked + 1; x <= limit; ++x) {
                if (counts[x] == 5) {
                    /* Append to the values array */
                    if (values_len == values_cap) {
                        size_t new_cap = values_cap ? values_cap * 2 : 32;
                        seq_t *tmp = (seq_t *)realloc(values,
                                       new_cap * sizeof(seq_t));
                        if (!tmp) {
                            free(counts);
                            return (seq_t)0;      /* allocation failure */
                        }
                        values = tmp;
                        values_cap = new_cap;
                    }
                    values[values_len++] = (seq_t)x;
                }
            }
            free(counts);
            max_checked = limit;

            if (values_len >= n) break;

            /* Increase the search bound for the next pass */
            limit <<= 1;
        }
    }

    return values[n - 1];
}
