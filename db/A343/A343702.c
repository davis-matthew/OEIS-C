/*
 * A343702  Numbers that are the sum of five positive cubes in two or more
 *          ways.
 * https://oeis.org/A343702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9762, u64 n <= 9762, u128 n <= 9762,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A343702: numbers expressible as sum of five positive cubes
   in two or more distinct ways.  This implementation uses a brute‑force
   search over all non‑decreasing 5‑tuples of cube bases and counts
   occurrences.  Whenever at least two different tuples yield the
   same sum, the sum is added to the sequence.  The search limit is
   increased incrementally until enough terms are found. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Static data for memoization */
static seq_t *term_arr = NULL;      /* array of computed terms   */
static size_t term_cap = 0;         /* allocated capacity       */
static size_t term_len = 0;         /* number of stored terms   */
static idx_t   computed_max = 0;    /* highest base value used   */

/* Helper: ensure we have at least n terms stored.  The search limit
   max_val is increased incrementally until the sequence contains
   n elements.  New terms are computed from scratch each time. */
static void ensure_terms(idx_t n)
{
    /* initial limit chosen to cover more than 10 000 terms comfortably */
    const idx_t INIT_MAX = 50;
    const idx_t INCR     = 10;

    if (computed_max == 0) {
        computed_max = INIT_MAX;
    }

    while (term_len < (size_t)n) {
        /* Allocate cubes array */
        idx_t    max = (idx_t)computed_max;
        size_t   len_cubes = (size_t)(max + 1);
        seq_t   *cubes   = (seq_t *)malloc(len_cubes * sizeof(seq_t));
        cubes[0] = 0;
        for (idx_t i = 1; i <= max; ++i) {
            seq_t v = (seq_t)i * (seq_t)i * (seq_t)i;
            cubes[i] = v;
        }

        /* Find maximum possible sum for this limit */
        seq_t   max_cube = cubes[max];
        size_t  max_sum  = (size_t)5 * (size_t)max_cube;

        /* Count occurrences of each sum */
        uint16_t *cnt = (uint16_t *)calloc(max_sum + 1, sizeof(uint16_t));
        if (!cnt) { free(cubes); return; }

        for (idx_t e = 1; e <= max; ++e) {
            seq_t ce = cubes[e];
            for (idx_t d = 1; d <= e; ++d) {
                seq_t cd = cubes[d];
                for (idx_t c = 1; c <= d; ++c) {
                    seq_t cc = cubes[c];
                    for (idx_t b = 1; b <= c; ++b) {
                        seq_t cb = cubes[b];
                        for (idx_t a = 1; a <= b; ++a) {
                            seq_t ca = cubes[a];
                            seq_t sum = ca + cb + cc + cd + ce;
                            ++cnt[(size_t)sum];
                        }
                    }
                }
            }
        }

        /* Prepare term array */
        if (term_arr == NULL || term_cap < max_sum + 1) {
            seq_t *tmp = (seq_t *)realloc(term_arr, (max_sum + 1) * sizeof(seq_t));
            term_arr = tmp;
            term_cap = max_sum + 1;
        }

        /* Store sums that occur at least twice, in ascending order */
        size_t pos = 0;
        for (size_t s = 0; s <= max_sum; ++s) {
            if (cnt[s] >= 2) {
                term_arr[pos++] = (seq_t)s;
            }
        }
        term_len = pos;

        /* Clean up */
        free(cubes);
        free(cnt);

        /* Increase search limit if needed */
        if (term_len < (size_t)n) {
            computed_max += INCR;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) { return 0; }          /* undefined; safeguard */
    ensure_terms(n);
    return term_arr[n - 1];
}
