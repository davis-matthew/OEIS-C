/*
 * A338643  Cycle lengths arising from interpreting sequence prefixes as graph
 *          node offsets.
 * https://oeis.org/A338643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8016, u16 n <= 50000, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 96 terms through n = 9656 (2026-11-01)
 * Helpers:   none
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

/* Return the next node index in the graph of 'm' nodes, given the
   current node 'i'.  Nodes are 0‑based. */
static inline idx_t
next_node(idx_t i, const idx_t *offsets, idx_t m)
{
    return (i + offsets[i]) % m;
}

/* Compute the length of the cycle containing node 0 in the graph
   defined by the first 'm' offsets.  Uses Floyd's cycle‑finding algorithm. */
static idx_t
cycle_len(const idx_t *offsets, idx_t m)
{
    idx_t tortoise = next_node(0, offsets, m);
    idx_t hare     = next_node(tortoise, offsets, m);

    /* Find a meeting point inside the cycle. */
    while (tortoise != hare) {
        tortoise = next_node(tortoise, offsets, m);
        hare     = next_node(next_node(hare, offsets, m), offsets, m);
    }

    /* Now tortoise==hare somewhere in the cycle.  Compute cycle length. */
    idx_t lam = 1;
    hare = next_node(tortoise, offsets, m);
    while (hare != tortoise) {
        hare = next_node(hare, offsets, m);
        ++lam;
    }
    return lam;
}

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
