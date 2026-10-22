/*
 * A338643  Cycle lengths arising from interpreting sequence prefixes as graph
 *          node offsets.
 * https://oeis.org/A338643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8016, u16 n <= 9656, u32 n <= 9656, u64 n <= 9656,
 *            u128 n <= 9656, big any n
 * Helpers:   cycle_len, next_node
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338643.
   a(1) = 1.
   For n > 1, a(n) is the length of the unique directed cycle that
   contains node 1 in the graph built from the prefix a(1)..a(n-1).
   Each node i (1‑based) points to ((i-1) + a(i)) mod (n-1) + 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cycle_len.h"
#include "../utils/next_node.h"

/* Static storage for the sequence values.  Values are stored as
   idx_t (uint64_t) so they can be cast to seq_t on return. */
static idx_t *a_vals = NULL;   /* offset 0 holds a(1) */
static idx_t  a_len  = 0;      /* number of values currently stored */
static idx_t  a_cap  = 0;      /* capacity of a_vals */

/* Ensure that a_vals can hold at least 'needed' elements. */
static void
ensure_a_cap(idx_t needed)
{
    if (a_cap < needed) {
        idx_t new_cap = a_cap ? a_cap * 2 : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        idx_t *new_ptr = (idx_t *)realloc(a_vals, new_cap * sizeof(idx_t));
        a_vals = new_ptr;
        a_cap  = new_cap;
    }
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    /* Initialize first value if necessary. */
    if (a_len == 0) {
        ensure_a_cap(1);
        a_vals[0] = 1;   /* a(1) = 1 */
        a_len = 1;
    }

    /* Extend the sequence up to index n (1‑based). */
    for (idx_t idx = a_len + 1; idx <= n; ++idx) {
        idx_t m   = idx - 1;            /* current prefix length */
        idx_t len = cycle_len(a_vals, m); /* cycle length of this prefix */
        ensure_a_cap(idx);
        a_vals[a_len] = len;    /* store a(idx) */
        a_len = idx;
    }

    return (seq_t)a_vals[n - 1];
}
