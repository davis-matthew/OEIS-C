/*
 * A379545  Triangle read by rows where row n lists powerful divisors d | n
 *          (i.e., d in A001694).
 * https://oeis.org/A379545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 461, u16 n <= 9728, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   alloc_seq_array, cmp_seq_t, factor_u64, free_seq_array,
 *            gen_powdiv_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation of OEIS A379545 (flattened triangle of powerful divisor rows).
 *
 * The triangle is defined as follows: row n lists all powerful divisors
 * d of n (i.e. d ∈ A001694 that divide n).  The sequence a(n) is the
 * terms of this triangle read row‑by‑row starting at n = 1.  For a given
 * index n we locate the row m that contains it, generate the list of
 * powerful divisors of m, and return the appropriate element.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper headers (provided by the build system) */
#include "../utils/factor_u64.h"          /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/alloc_seq_array.h"     /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/free_seq_array.h"      /* void free_seq_array(seq_t *p) */
#include "../utils/cmp_seq_t.h"           /* int cmp_seq_t(const void *p1, const void *p2) */
#include "../utils/gen_powdiv_rec.h"

/* ------------------------------------------------------------------ */
/* Count powerful divisors of n (n ≥ 1).  Uses factor_u64 to factor n. */
static size_t num_powdiv(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    size_t total = 1;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] >= 2)
            total *= (size_t)ex[i];        /* e choices: 0 or 2..e */
    return total;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Return sorted array of powerful divisors of m (m ≥ 1).  Caller must
 * free the array with free_seq_array.  The number of elements is
 * returned via *len_out. */
static seq_t *get_powdivs(uint64_t m, size_t *len_out)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);

    size_t total = num_powdiv(m);
    seq_t *arr = alloc_seq_array(total);

    size_t idx = 0;
    gen_powdiv_rec(0, cnt, pr, ex, (seq_t)1, arr, &idx);

    /* sort ascending */
    qsort(arr, total, sizeof(seq_t), cmp_seq_t);

    *len_out = total;
    return arr;
}

/* ------------------------------------------------------------------ */
/* Cached cumulative counts of powerful divisors up to each row. */
static idx_t *cum_counts = NULL;   /* array of cumulative counts */
static size_t cum_cap   = 0;       /* allocated size of cum_counts */
static idx_t cum_len    = 0;       /* last row index computed */

/* Ensure cum_counts is allocated up to 'need' (inclusive). */
static void ensure_cum(idx_t need)
{
    if (cum_counts == NULL) {
        cum_cap = 64;
        cum_counts = (idx_t *)malloc(sizeof(idx_t) * cum_cap);
        cum_counts[0] = 0;      /* cumulative count before row 1 */
        cum_len = 0;
    }

    while (cum_len < need) {
        ++cum_len;                                      /* next row index */
        size_t cnt = num_powdiv((uint64_t)cum_len);      /* divisors of this row */
        if (cum_len >= cum_cap) {
            cum_cap *= 2;
            cum_counts = (idx_t *)realloc(cum_counts, sizeof(idx_t) * cum_cap);
        }
        cum_counts[cum_len] = cum_counts[cum_len - 1] + (idx_t)cnt;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, just to be safe */

    /* Pre‑compute cumulative counts up to necessary row. */
    ensure_cum(n);

    /* Binary search for the row containing the n-th element. */
    idx_t low = 1, high = cum_len, m = cum_len;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        if (cum_counts[mid] >= n) {
            m = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* 1‑based position within the found row */
    idx_t offset = n - cum_counts[m - 1];

    /* Generate the powerful divisors of row m (sorted). */
    size_t len;
    seq_t *divs = get_powdivs((uint64_t)m, &len);
    seq_t result = divs[offset - 1];
    free_seq_array(divs);

    return result;
}
