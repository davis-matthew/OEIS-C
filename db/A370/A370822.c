/*
 * A370822  Lexicographically earliest sequence of positive integers such that
 *          all equal terms appear at mutually coprime indices.
 * https://oeis.org/A370822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_idx_array_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithmic construction of OEIS sequence A370822:
 * For each positive integer n select the
 * smallest value v such that every previous index i with a(i)=v
 * satisfies gcd(i,n)=1.  The function is memoised: calls are
 * answered in any order and the sequence is computed lazily.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ensure_idx_array_capacity.h"

/* Helper structure for storing the list of indices at which a value occurs */
struct IndexList {
    idx_t *idx;   /* dynamic array of indices */
    size_t sz;    /* number of indices stored */
    size_t cap;   /* capacity of the array */
};

/* Compute gcd for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != (idx_t)0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Ensure there is space for at least 'req' IndexList entries */
static void ensure_indexlist_array_capacity(struct IndexList **ptr,
                                            size_t *cap,
                                            size_t req)
{
    if (*cap >= req) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap;
    while (new_cap < req) new_cap <<= 1;
    struct IndexList *new_ptr =
        (struct IndexList *)realloc(*ptr, new_cap * sizeof **ptr);
    /* initialise the newly created elements */
    for (size_t i = *cap; i < new_cap; ++i) {
        new_ptr[i].idx = NULL;
        new_ptr[i].sz  = 0;
        new_ptr[i].cap = 0;
    }
    *ptr = new_ptr;
    *cap = new_cap;
}

/* Append an index to an IndexList */
static void append_index(struct IndexList *list, idx_t val)
{
    if (list->cap == list->sz) {
        size_t new_cap = (list->cap == 0) ? 4 : list->cap << 1;
        idx_t *new_idx = (idx_t *)realloc(list->idx, new_cap * sizeof *new_idx);
        list->idx = new_idx;
        list->cap = new_cap;
    }
    list->idx[list->sz++] = val;
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *values = NULL;                 /* computed a(i) (0‑based) */
    static size_t values_cap = 0;                /* capacity of 'values' */
    static idx_t max_computed = 0;               /* largest index computed */

    static struct IndexList *value_lists = NULL; /* lists of indices per value */
    static size_t value_lists_cap = 0;            /* capacity of 'value_lists' */

    if (n == (idx_t)0) /* undefined, just return 0 */
        return (seq_t)0;

    /* Grow values array if necessary */
    if ((size_t)n > values_cap) {
        size_t new_cap = (values_cap == 0) ? 16 : values_cap << 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_vals = (seq_t *)realloc(values, new_cap * sizeof *new_vals);
        values = new_vals;
        values_cap = new_cap;
        /* initialise new slots to zero (unknown) */
        for (size_t i = max_computed; i < new_cap; ++i)
            values[i] = (seq_t)0;
    }

    /* Compute values up to n if not already done */
    while (max_computed < n) {
        idx_t idx = max_computed + 1;  /* index to compute (1‑based) */
        seq_t val = (seq_t)1;          /* candidate value */

        /* Search for smallest valid value */
        while (true) {
            size_t val_usize = (size_t)val;   /* to index value_lists */
            if (val_usize >= value_lists_cap) {
                /* Ensure array is large enough for this value */
                ensure_indexlist_array_capacity(&value_lists,
                                                &value_lists_cap,
                                                val_usize + 1);
            }
            struct IndexList *list = &value_lists[val_usize];
            bool ok = true;
            for (size_t j = 0; j < list->sz; ++j) {
                if (gcd_idx(list->idx[j], idx) != (idx_t)1) {
                    ok = false;
                    break;
                }
            }
            if (ok) break;          /* found suitable value */
            ++val;                   /* try next candidate */
        }

        /* Store result and record index for this value */
        values[idx - 1] = val;
        size_t val_usize = (size_t)val;
        append_index(&value_lists[val_usize], idx);

        ++max_computed;
    }

    return values[n - 1];
}
