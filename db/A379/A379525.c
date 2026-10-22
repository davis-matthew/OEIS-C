/*
 * A379525  a(1) = 1. For n > 1, if a(n-1) is a novel term, a(n) =
 *          A003132(a(n-1)). If a(n-1) has been seen k (>1) times already,
 *          a(n) = k*a(n-1).
 * https://oeis.org/A379525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 138, u16 n <= 1569, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_sq_dec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A379525:  
 *    a(1) = 1.  For n > 1,
 *      if a(n‑1) is novel, a(n) = A003132(a(n‑1))   (sum of squares of decimal
 *         digits),
 *      if a(n‑1) has been seen k (>1) times, a(n) = k·a(n‑1).
 *  The code memoises all terms and counts of each value, works for any
 *  integer type supplied for seq_t and idx_t, and compiles in both C11
 *  and C++17.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sum_sq_dec.h"

/* Helper struct to record how many times a value has appeared. */
struct val_count {
    seq_t val;
    int   count;
};

/* Static state for memoisation. */
static seq_t      *terms      = NULL;
static size_t      terms_len  = 0;   /* largest index initialised (1‑based) */
static size_t      terms_cap  = 0;

static struct val_count *vc_arr = NULL;
static size_t              vc_len  = 0;
static size_t              vc_cap  = 0;

/* Locate the counted entry for a value; NULL if not present. */
static struct val_count *find_val(seq_t v)
{
    for (size_t i = 0; i < vc_len; ++i) {
        if (vc_arr[i].val == v) return &vc_arr[i];
    }
    return NULL;
}

/* Ensure terms[] has capacity for 'need' elements. */
static void ensure_terms(size_t need)
{
    if (terms_cap < need) {
        size_t newcap = terms_cap ? terms_cap * 2 : 8;
        while (newcap < need) newcap *= 2;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }
}

/* Ensure vc_arr[] has capacity for 'need' entries. */
static void ensure_val_counts(size_t need)
{
    if (vc_cap < need) {
        size_t newcap = vc_cap ? vc_cap * 2 : 8;
        while (newcap < need) newcap *= 2;
        vc_arr = (struct val_count *)realloc(vc_arr,
                                              newcap * sizeof(struct val_count));
        vc_cap = newcap;
    }
}

/* Record that a value has just appeared one more time. */
static void add_value(seq_t v)
{
    struct val_count *p = find_val(v);
    if (p) {
        ++p->count;
    } else {
        ensure_val_counts(vc_len + 1);
        vc_arr[vc_len].val    = v;
        vc_arr[vc_len].count = 1;
        ++vc_len;
    }
}

/* Public entry point required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; ignore zero. */

    /* Expand cache up to index n. */
    if (terms_len < (size_t)n) {
        ensure_terms((size_t)n + 1);      /* 1‑based indexing */

        /* Initialise first term on first call. */
        if (terms_len == 0) {
            terms[1] = (seq_t)1;
            add_value(terms[1]);
            terms_len = 1;
        }

        /* Generate terms until we reach n. */
        while (terms_len < (size_t)n) {
            seq_t prev   = terms[terms_len];
            struct val_count *p = find_val(prev);
            int k = p ? p->count : 0;     /* times prev has already appeared */

            seq_t next;
            if (k == 1) {                 /* novel term */
                next = sum_sq_dec(prev);
            } else {                      /* seen before */
                next = k * prev;
            }

            ++terms_len;
            terms[terms_len] = next;
            add_value(next);
        }
    }

    return terms[(size_t)n];
}
