/*
 * A371248  Square array A(n, k), n, k > 0, read and filled in the greedy way
 *          by upward antidiagonals with distinct positive integers such that
 *          for any k > 0, the terms in the k-th column are congruent modulo
 *          k.
 * https://oeis.org/A371248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 241, u16 n <= 9574, u32 n <= 9574, u64 n <= 9574,
 *            u128 n <= 9574, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of the square array A371248.
   The sequence a(n), n ≥ 1, is read from the array along upward
   antidiagonals: for each sum s = n + k the cells (n,k) are visited
   with decreasing n (increasing k).  For every column k the entries
   are congruent modulo k.  This implementation generates the terms
   lazily and memoises them. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

static seq_t *rem = NULL;
static size_t rem_cap = 0;
#define REM_UNINIT ((seq_t)-1)

static unsigned char *used = NULL;
static size_t used_cap = 0;

static idx_t diag_sum = 2;          /* current sum i + k of the diagonal */
static size_t pos_in_diag = 0;      /* position inside that diagonal (0 .. sum-2) */
static idx_t smallest_unused = 1;   /* smallest positive integer not yet used */

/* Ensure that 'used' can index values up to 'val' (inclusive). */
static void
ensure_used_capacity(seq_t val)
{
    size_t idx_val = (size_t)val;
    if (idx_val < used_cap)
        return;
    size_t old = used_cap;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= idx_val)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp)
        return;
    used = tmp;
    memset(used + old, 0, new_cap - old);
    used_cap = new_cap;
}

/* Ensure that 'rem' can index columns up to 'j' (inclusive). */
static void
ensure_rem_capacity(size_t j)
{
    if (j < rem_cap)
        return;
    size_t old = rem_cap;
    size_t new_cap = rem_cap ? rem_cap << 1 : 16;
    while (new_cap <= j)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(rem, new_cap * sizeof(seq_t));
    if (!tmp)
        return;
    rem = tmp;
    for (size_t k = old; k < new_cap; ++k)
        rem[k] = REM_UNINIT;
    rem_cap = new_cap;
}

/* Ensure that 'cache' can store at least 'need' entries. */
static void
ensure_cache_capacity(size_t need)
{
    if (cache_size >= need)
        return;
    size_t old = cache_cap;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0 in the OEIS entry */

    /* Generate terms until we have at least n cached. */
    while (cache_size < (size_t)n) {
        /* Current cell coordinates.  Diagonal sum starts at 2 for (1,1). */
        idx_t pos = (idx_t)pos_in_diag;   /* 0‑based position inside diagonal */
        idx_t col = pos + 1;              /* column k  (>=1)   */
        /* row = diag_sum - 1 - pos;  not needed directly. */

        /* Ensure remainder table has room for this column. */
        ensure_rem_capacity((size_t)col);

        seq_t value;

        if (rem[col] == REM_UNINIT) {
            /* First entry in this column: choose the smallest unused integer. */
            seq_t cand = smallest_unused;
            ensure_used_capacity(cand);
            while (used[(size_t)cand])
                ++cand;
            value = cand;
            rem[col] = value % (seq_t)col;   /* column remainder */
            used[(size_t)cand] = 1;

            /* Advance to the next unused integer. */
            if ((seq_t)smallest_unused == cand) {
                do {
                    ++smallest_unused;
                    ensure_used_capacity(smallest_unused);
                } while (used[(size_t)smallest_unused]);
            }
        } else {
            /* Column already has a fixed remainder. */
            seq_t r = rem[col];
            seq_t cand = (r == 0) ? (seq_t)col : r;
            ensure_used_capacity(cand);
            while (used[(size_t)cand]) {
                cand += (seq_t)col;
                ensure_used_capacity(cand);
            }
            value = cand;
            used[(size_t)cand] = 1;

            /* Update smallest_unused if necessary. */
            if ((seq_t)smallest_unused == cand) {
                do {
                    ++smallest_unused;
                    ensure_used_capacity(smallest_unused);
                } while (used[(size_t)smallest_unused]);
            }
        }

        /* Store the new term. */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = value;

        /* Advance to the next position. */
        ++pos_in_diag;
        if (pos_in_diag == (size_t)(diag_sum - 1)) {
            ++diag_sum;
            pos_in_diag = 0;
        }
    }

    return cache[n - 1];
}
