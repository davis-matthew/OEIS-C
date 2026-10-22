/*
 * A378912  Irregular triangle read by rows: row n lists all positive m such
 *          that sigma(m) divides n, where sigma is the sum-of-divisors
 *          function (A000203).
 * https://oeis.org/A378912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1026, u16 n <= 9387, u32 n <= 9387, u64 n <= 9387,
 *            u128 n <= 9387, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A378912
 * Definition: row n lists all positive m such that sigma(m) divides n,
 * where sigma is the sum‑of‑divisors function.  The sequence a(n) is the
 * triangle read by rows (row 1 → first term, row 2 → second term, …).
 * This implementation builds the sequence incrementally by caching, so
 * yielding a(n) for any n ≥ 1 in O(n) time (worst case) and within
 * the required memory limits.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* ---------- data structures ---------- */
typedef struct {
    idx_t *data;   /* array of ms sharing the same sigma value */
    size_t len;    /* number of elements currently stored   */
    size_t cap;    /* allocated capacity                   */
} SList;

static idx_t *output = NULL;       /* flattened sequence (idx_t values)   */
static size_t out_len = 0;         /* number of terms already stored     */
static size_t out_cap = 0;         /* allocated capacity of output       */

static SList *slist = NULL;       /* array indexed by sigma value       */
static size_t slist_cap = 0;        /* size of slist array                */

static idx_t last_row_processed = 0;   /* largest processed row             */

/* ---------- helper functions ---------- */

/* Add m to the list whose sigma value is s.  The list is kept sorted
 * because m is added in increasing order.  */
static void add_to_slist(idx_t s, idx_t m)
{
    /* grow the slist array if necessary */
    if ((size_t)s + 1 > slist_cap) {
        size_t new_cap = (size_t)s + 1;
        slist = (SList *)realloc(slist, new_cap * sizeof(SList));
        for (size_t i = slist_cap; i < new_cap; ++i) {
            slist[i].data = NULL;
            slist[i].len  = 0;
            slist[i].cap  = 0;
        }
        slist_cap = new_cap;
    }

    SList *L = &slist[s];
    if (L->cap == 0) {
        L->cap   = 8;
        L->data  = (idx_t *)malloc(L->cap * sizeof(idx_t));
    } else if (L->len == L->cap) {
        L->cap <<= 1;
        L->data = (idx_t *)realloc(L->data, L->cap * sizeof(idx_t));
    }
    L->data[L->len++] = m;
}

/* Append a value to the global output array, enlarging when full. */
static void push_output(idx_t val)
{
    if (out_cap == 0 || out_len == out_cap) {
        out_cap = (out_cap == 0) ? 8 : out_cap << 1;
        output = (idx_t *)realloc(output, out_cap * sizeof(idx_t));
    }
    output[out_len++] = val;
}

/* Merge all lists whose sigma divides the given row number, outputting
 * the merged, ascending ms.  */
static void process_row(idx_t row)
{
    /* determine divisors of row */
    idx_t divs[256];
    size_t dcnt = 0;
    for (idx_t d = 1; d * d <= row; ++d) {
        if (row % d == 0) {
            divs[dcnt++] = d;
            if (d * d != row)
                divs[dcnt++] = row / d;
        }
    }

    /* gather pointers to the relevant lists */
    SList *lists[256];
    size_t  pos[256];
    size_t  nlists = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        if ((size_t)d < slist_cap && slist[d].len > 0) {
            lists[nlists]   = &slist[d];
            pos[nlists]     = 0;
            ++nlists;
        }
    }

    /* simple k‑way merge of the selected lists */
    while (nlists > 0) {
        idx_t best = 0;
        size_t best_i = 0;
        bool   first = true;

        for (size_t i = 0; i < nlists; ++i) {
            idx_t v = lists[i]->data[pos[i]];
            if (first || v < best) {
                best   = v;
                best_i = i;
                first  = false;
            }
        }

        push_output(best);
        ++pos[best_i];
        if (pos[best_i] >= lists[best_i]->len) {
            /* remove exhausted list from the pool */
            if (best_i != nlists - 1) {
                lists[best_i] = lists[nlists - 1];
                pos[best_i]   = pos[nlists - 1];
            }
            --nlists;
        }
    }
}

/* Extend the precomputed sequence until it contains at least `target`
 * terms. */
static void extend(idx_t target)
{
    while (out_len < target) {
        ++last_row_processed;                     /* next row index   */
        seq_t sigma_val = sigma_of(last_row_processed); /* sum of divisors */
        idx_t sigma_int = (idx_t)sigma_val;        /* cast for indexing */

        add_to_slist(sigma_int, last_row_processed);
        process_row(last_row_processed);
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* guard for out‑of‑range calls */
    if (n > out_len)        /* compute until we have enough terms */
        extend(n);
    return (seq_t)output[n - 1];
}
