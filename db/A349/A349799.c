/*
 * A349799  Numbers k such that the k-th composition in standard order is
 *          weakly alternating but has at least two adjacent equal parts.
 * https://oeis.org/A349799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A349799 by enumerating all compositions in standard order
 * (graded reverse-lexicographic) and selecting those that are weakly
 * alternating with at least one pair of equal adjacent parts. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *values = NULL;     // cache of a(n) values
static size_t vals_len = 0;      // number of values cached
static size_t vals_cap = 0;      // capacity of cache

static idx_t max_weight_generated = 0; // last weight for which we generated comps
static idx_t next_k = 1;      // next composition index (k) to assign

/* Grow the cache when needed. */
static void ensure_capacity(size_t needed)
{
    if (vals_cap < needed) {
        size_t new_cap = vals_cap ? vals_cap * 2 : 32;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        values = tmp;
        vals_cap = new_cap;
    }
}

/* Append a value to the cache. */
static void push_value(seq_t v)
{
    ensure_capacity(vals_len + 1);
    values[vals_len++] = v;
}

/* Test whether a composition (given in array comp[0..len-1]) is weakly alternating
 * and has at least one pair of equal adjacent parts. */
static bool matches(const int comp[], size_t len)
{
    if (len < 2) return false;          // need at least one adjacent pair

    /* Check for adjacent equal parts. */
    bool has_equal = false;
    for (size_t i = 0; i + 1 < len; ++i)
        if (comp[i] == comp[i + 1]) { has_equal = true; break; }
    if (!has_equal) return false;

    /* Weakly alternating: try both starting directions. */
    /* Pattern 1: start with <=, then >=, then <= ... */
    bool ok1 = true;
    for (size_t i = 0; i + 1 < len; ++i) {
        if ((i & 1) == 0) {
            if (comp[i] > comp[i + 1]) { ok1 = false; break; }
        } else {
            if (comp[i] < comp[i + 1]) { ok1 = false; break; }
        }
    }
    /* Pattern 2: start with >=, then <=, then >= ... */
    bool ok2 = true;
    for (size_t i = 0; i + 1 < len; ++i) {
        if ((i & 1) == 0) {
            if (comp[i] < comp[i + 1]) { ok2 = false; break; }
        } else {
            if (comp[i] > comp[i + 1]) { ok2 = false; break; }
        }
    }
    return ok1 || ok2;
}

/* Recursively generate all compositions of a given remaining weight.
 * parts[depth] holds the part currently being considered (from right to left).
 * When rem == 0 we have a full composition stored in reverse order in parts[0..depth-1].
 */
static void gen_recurse(int rem, int depth, int parts[])
{
    if (rem == 0) {
        /* Build the composition in correct order. */
        size_t len = (size_t)depth;
        int comp[32];
        for (size_t i = 0; i < len; ++i)
            comp[i] = parts[len - 1 - i];

        if (matches(comp, len))
            push_value((seq_t)next_k);
        ++next_k;
        return;
    }
    for (int last = rem; last >= 1; --last) {
        parts[depth] = last;
        gen_recurse(rem - last, depth + 1, parts);
    }
}

/* Generate compositions for weights until we have cached at least n values. */
static void grow_cache(idx_t n)
{
    while (vals_len < n) {
        int w = (int)(max_weight_generated + 1);
        int parts[32];
        gen_recurse(w, 0, parts);
        ++max_weight_generated;
    }
}

/* Main accessor.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (vals_len < (size_t)n) grow_cache(n);
    return values[n - 1];
}
