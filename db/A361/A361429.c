/*
 * A361429  a(n) is the smallest positive number not among the terms between
 *          a(n-1) and the most recent previous term whose value appears with
 *          the same frequency (inclusive); if no such term exists, set
 *          a(n)=1; a(1)=1.
 * https://oeis.org/A361429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361429
   a(1) = 1
   For n > 1: let f be the current frequency of a(n‑1). Search for the
   most recent index j < n‑1 where a(j) has the same frequency f after
   j has been inserted.  If no such j exists set a(n)=1; otherwise
   a(n) is the smallest positive integer not appearing among
   a(j), a(j+1), …, a(n‑1).  The function below implements this
   recurrence with memoisation and works for any of the supported
   integer types. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    seq_t val;
    uint64_t cnt;         /* current frequency of val */
} valrec_t;

/* Memoised sequence (1‑based indexing) */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;

/* Mapping from a distinct value to its frequency record */
static valrec_t *valtab = NULL;
static size_t val_cap = 0;
static size_t val_len = 0;

/* For each term, the index in valtab of its value */
static size_t *val_of_term = NULL;
static size_t val_of_term_cap = 0;

/* Temporary array for presence test when searching the smallest
   missing positive integer. */
static bool *present = NULL;
static size_t present_cap = 0;

/* Ensure enough capacity for seq[1..n] and val_of_term[1..n] */
static void ensure_seq_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;   /* index 0 unused */
    if (need > seq_cap) {
        seq = (seq_t *)realloc(seq, need * sizeof(seq_t));
        val_of_term = (size_t *)realloc(val_of_term, need * sizeof(size_t));
        seq_cap = need;
    }
    if (need > val_of_term_cap) {
        val_of_term = (size_t *)realloc(val_of_term, need * sizeof(size_t));
        val_of_term_cap = need;
    }
}

/* Ensure capacity to add another distinct value */
static void ensure_valtab_capacity(void)
{
    if (val_len >= val_cap) {
        size_t new_cap = val_cap ? val_cap * 2 : 16;
        valtab = (valrec_t *)realloc(valtab, new_cap * sizeof(valrec_t));
        val_cap = new_cap;
    }
}

/* Increment the frequency of value v, returning its index in valtab.
   If v is new, a new entry is created. */
static void inc_value_count(seq_t v, size_t *idx_out)
{
    for (size_t i = 0; i < val_len; ++i) {
        if (valtab[i].val == v) {
            ++valtab[i].cnt;
            *idx_out = i;
            return;
        }
    }
    /* New distinct value */
    ensure_valtab_capacity();
    valtab[val_len].val = v;
    valtab[val_len].cnt = 1;
    *idx_out = val_len;
    ++val_len;
}

/* Find the smallest positive integer not in seq[l..r] (inclusive). */
static seq_t smallest_missing(idx_t l, idx_t r)
{
    size_t length = (size_t)(r - l + 1);
    size_t limit  = length + 1;   /* missing value <= limit */

    if (limit + 1 > present_cap) {
        present = (bool *)realloc(present, (limit + 2) * sizeof(bool));
        present_cap = limit + 2;
    }
    memset(present, 0, (limit + 2) * sizeof(bool));

    for (idx_t k = l; k <= r; ++k) {
        seq_t v = seq[(size_t)k];
        if (v <= (seq_t)(limit + 1))
            present[(size_t)v] = true;
    }

    seq_t missing = 1;
    while (missing <= (seq_t)(limit + 1) && present[(size_t)missing])
        ++missing;
    return missing;
}

/* Main function requested by the interface */
seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for n=0; return 0 as safeguard */
        return 0;

    ensure_seq_capacity(n);

    /* Compute terms up to n if necessary */
    while (seq_len < n) {
        idx_t i = seq_len + 1;   /* next index to compute */
        seq_t val;

        if (i == 1) {
            val = 1;
        } else {
            size_t vidx_prev = val_of_term[(size_t)(i - 1)];
            uint64_t freq_last = valtab[vidx_prev].cnt;

            /* Search for most recent earlier index with same frequency */
            idx_t m = 0;
            for (idx_t j = i - 2; j >= 1; --j) {
                size_t vidx_j = val_of_term[(size_t)j];
                if (valtab[vidx_j].cnt == freq_last) {
                    m = j;
                    break;
                }
            }

            if (m == 0)
                val = 1;
            else
                val = smallest_missing(m, i - 1);
        }

        /* Record the new term */
        size_t vidx;
        inc_value_count(val, &vidx);
        seq[(size_t)i] = val;
        val_of_term[(size_t)i] = vidx;
        seq_len = i;
    }

    return seq[(size_t)n];
}
