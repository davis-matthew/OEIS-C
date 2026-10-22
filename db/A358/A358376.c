/*
 * A358376  Numbers k such that the k-th standard ordered rooted tree is
 *          lone-child-avoiding (counted by A005043).
 * https://oeis.org/A358376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358376 – Numbers k such that the k‑th standard ordered rooted
 * tree (see A005043) is lone‑child‑avoiding.  The code enumerates the
 * trees in standard order and selects those with no node having exactly
 * one child.  a(n) is the nth such index (offset 1). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/get_parts.h"

/* Helper: convert rank r (= n-1) to the composition parts of the root. */

/* Memoised test: true if tree n is lone‑child‑avoiding. */
static signed char *lca_cache = NULL;   /* -1: unknown, 0: false, 1: true */
static size_t lca_cap = 0;

static inline void ensure_lca_cap(idx_t n)
{
    if (n < lca_cap) return;
    size_t new_cap = (n + 1) * 2;
    if (new_cap < 8) new_cap = 8;
    signed char *new_arr = (signed char *)malloc(new_cap * sizeof(signed char));
    if (!new_arr) exit(1);                     /* fatal allocation error */
    for (size_t i = 0; i < new_cap; ++i) new_arr[i] = -1;
    if (lca_cache) {
        memcpy(new_arr, lca_cache, lca_cap * sizeof(signed char));
        free(lca_cache);
    }
    lca_cache = new_arr;
    lca_cap = new_cap;
}

static inline bool is_lca(idx_t n)
{
    ensure_lca_cap(n);
    signed char ev = lca_cache[n];
    if (ev != -1) return ev;

    /* Leaf node */
    if (n == 1) {
        lca_cache[n] = 1;
        return true;
    }

    /* Root composition */
    idx_t r = n - 1;
    idx_t parts[64], len;
    get_parts(r, parts, &len);

    /* Root with exactly one child is forbidden */
    if (len == 1) {
        lca_cache[n] = 0;
        return false;
    }

    /* Recursively check children */
    for (idx_t i = 0; i < len; ++i) {
        if (!is_lca(parts[i])) {
            lca_cache[n] = 0;
            return false;
        }
    }
    lca_cache[n] = 1;
    return true;
}

/* Cache of found a(n) values */
static seq_t *vals = NULL;   /* dynamic array of seq_t */
static size_t vals_size = 0;
static size_t vals_cap = 0;
static idx_t last_checked = 0;

static inline void ensure_vals_cap(size_t needed)
{
    if (needed <= vals_cap) return;
    size_t new_cap = vals_cap * 2;
    if (new_cap < 8) new_cap = 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);                     /* fatal error */
    vals = new_arr;
    vals_cap = new_cap;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    while (vals_size < n) {
        ++last_checked;
        if (is_lca(last_checked)) {
            ensure_vals_cap(vals_size + 1);
            vals[vals_size++] = (seq_t)last_checked;
        }
    }
    return vals[n - 1];
}
