/*
 * A353847  Composition run-sum transformation in terms of standard
 *          composition numbers. The a(k)-th composition in standard order is
 *          the sequence of run-sums of the k-th composition in standard
 *          order. Takes each index of a row of A066099 to the index of the
 *          row consisting of its run-sums.
 * https://oeis.org/A353847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the run‑sum composition of the n‑th composition
 *            * in standard order (A066099)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   index_of_comp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/index_of_comp.h"

/* allocate all compositions up to weight MAX_SUM.
 * total number of compositions = 2^MAX_SUM.
 */
#define MAX_SUM 20

typedef struct {
    uint8_t len;
    uint8_t parts[MAX_SUM];
} comp_t;

/* global array of all compositions in standard (graded reverse‑lexicographic) order */
static comp_t *all_comps = NULL;
static size_t total_comps = 0;

/* recursive generator of compositions of weight w.
 * prefix holds current parts, len is current length.
 * out is the destination array, idx is the current position.
 */
static void generate_comps(int w, uint8_t *prefix, uint8_t len,
                            comp_t *out, size_t *idx)
{
    if (w == 0) {
        out[*idx].len = len;
        memcpy(out[*idx].parts, prefix, len);
        (*idx)++;
        return;
    }
    for (int a = w; a >= 1; --a) {
        prefix[len] = (uint8_t)a;
        generate_comps(w - a, prefix, len + 1, out, idx);
    }
}

/* initialise the global compositions array lazily */
static void init_compositions(void)
{
    if (all_comps != NULL)
        return; /* already initialised */

    total_comps = 1U << MAX_SUM;    /* 2^MAX_SUM compositions */
    all_comps = (comp_t*)malloc(sizeof(comp_t) * total_comps);
    size_t idx = 0;
    uint8_t prefix[MAX_SUM];

    /* generate compositions for each weight 0..MAX_SUM in increasing order */
    for (int w = 0; w <= MAX_SUM; ++w) {
        generate_comps(w, prefix, 0, all_comps, &idx);
    }
}

/* main function required by OEIS */
seq_t a(idx_t n)
{
    init_compositions();   /* ensures all_comps is ready */

    if (n >= (idx_t)total_comps)
        return (seq_t)n;   /* fallback: return n if out of precomputed range */

    /* retrieve the n-th composition */
    const comp_t *c = &all_comps[n];

    /* build run‑sum composition */
    uint8_t run_parts[MAX_SUM];
    uint8_t run_len = 0;
    int i = 0;
    while (i < c->len) {
        int cur = c->parts[i];
        int sum = cur;
        ++i;
        while (i < c->len && c->parts[i] == cur) {
            sum += cur;
            ++i;
        }
        run_parts[run_len++] = (uint8_t)sum;
    }

    /* compute index of the run‑sum composition */
    return index_of_comp(run_parts, run_len);
}
