/*
 * A367849  Lexicographically earliest infinite sequence of positive integers
 *          such that for each n, the values in a path of locations starting
 *          from any i=n are all distinct, where jumps are allowed from
 *          location i to i+a(i).
 * https://oeis.org/A367849
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

/* Implements A367849 – lexicographically earliest infinite sequence of
 * positive integers such that for each n, the values along any forward
 * jump path starting at n are all distinct.  The code builds the
 * sequence incrementally; each call may compute any required index in
 * arbitrary order.  It uses only integer arithmetic and standard
 * dynamic allocation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* auxiliary structure for dynamic predecessor lists */
typedef struct {
    idx_t *data;  /* dynamic array of predecessors */
    size_t sz;    /* number of elements stored */
    size_t cap;   /* allocated capacity */
} pred_list_t;

/* global dynamic arrays (1‑based indexing) */
static seq_t      *A      = NULL;       /* sequence values */
static pred_list_t *preds = NULL;       /* predecessor lists */
static uint32_t   *vis_time = NULL;     /* visited timestamps for DFS */

static idx_t cap  = 0;  /* allocated capacity (always > 0) */
static idx_t sz   = 0;  /* highest index computed so far */

/* per‑index scratch arrays for ancestor collection */
static idx_t *anc_arr  = NULL;
static size_t anc_cap  = 0;
static idx_t *stack_arr = NULL;
static size_t stack_cap = 0;
static uint32_t cur_vis_time = 1;   /* incremented per DFS */

/* Ensure that arrays have room for indices 0 … n (inclusive) */
static void
ensure_capacity(idx_t n)
{
    if (n < cap) return;
    idx_t new_cap = cap ? cap << 1 : 4;
    while (new_cap <= n) new_cap <<= 1;

    A       = (seq_t*)realloc(A,       sizeof(seq_t)       * new_cap);
    preds   = (pred_list_t*)realloc(preds, sizeof(pred_list_t) * new_cap);
    vis_time = (uint32_t*)realloc(vis_time, sizeof(uint32_t) * new_cap);

    for (idx_t i = cap; i < new_cap; ++i) {
        preds[i].data = NULL;
        preds[i].sz   = 0;
        preds[i].cap  = 0;
        vis_time[i]   = 0;
    }
    cap = new_cap;
}

/* Append an element to a dynamic predecessor list */
static void
push_pred(pred_list_t *pl, idx_t v)
{
    if (pl->cap == 0) {
        pl->cap = 4;
        pl->data = (idx_t*)malloc(sizeof(idx_t) * pl->cap);
    } else if (pl->sz == pl->cap) {
        pl->cap <<= 1;
        pl->data = (idx_t*)realloc(pl->data, sizeof(idx_t) * pl->cap);
    }
    pl->data[pl->sz++] = v;
}

/* Compute all ancestors of node n (excluding n) and store them in anc_arr */
static void
collect_ancestors(idx_t n, size_t *anc_sz)
{
    if (anc_cap < cap) {
        anc_arr = (idx_t*)realloc(anc_arr, sizeof(idx_t) * cap);
        stack_arr = (idx_t*)realloc(stack_arr, sizeof(idx_t) * cap);
        anc_cap = stack_cap = cap;
    }
    size_t sp = 0;        /* stack pointer */
    size_t aa = 0;        /* ancestor count */

    ++cur_vis_time;                /* new timestamp for this DFS */
    vis_time[n] = cur_vis_time;
    stack_arr[sp++] = n;           /* start from n itself */

    while (sp > 0) {
        idx_t u = stack_arr[--sp];
        pred_list_t *pl = &preds[u];
        for (size_t k = 0; k < pl->sz; ++k) {
            idx_t p = pl->data[k];
            if (vis_time[p] == cur_vis_time) continue;
            vis_time[p] = cur_vis_time;
            stack_arr[sp++] = p;
            anc_arr[aa++] = p;
        }
    }
    *anc_sz = aa;
}

/* Test whether candidate value v is forbidden when assigning to position i */
static bool
candidate_forbidden(idx_t i, seq_t v, size_t anc_sz)
{
    for (size_t k = 0; k < anc_sz; ++k) {
        idx_t j = anc_arr[k];
        if (A[j] == v && preds[j].sz > 0) {
            return true;
        }
    }
    return false;
}

/* Public routine: return a(n) */
seq_t a(idx_t n)
{
    /* Ensure capacity for the requested index and scratch arrays */
    if (n >= cap) ensure_capacity(n);
    if (sz == 0) {
        /* initial global setup for indices < cap */
        for (idx_t i = 0; i < cap; ++i) {
            preds[i].data = NULL;
            preds[i].sz   = 0;
            preds[i].cap  = 0;
            vis_time[i]   = 0;
        }
    }

    /* Compute all missing values up to n */
    for (idx_t i = sz + 1; i <= n; ++i) {

        /* Find all ancestors of i */
        size_t anc_sz;
        collect_ancestors(i, &anc_sz);

        /* Search for the smallest admissible value */
        seq_t cand = 1;
        while (candidate_forbidden(i, cand, anc_sz))
            ++cand;

        /* Store the value */
        A[i] = cand;

        /* Add this index as a predecessor of its successor */
        idx_t succ = i + (idx_t)cand;
        if (succ >= cap) ensure_capacity(succ);
        push_pred(&preds[succ], i);

        sz = i;
    }
    return A[n];
}
