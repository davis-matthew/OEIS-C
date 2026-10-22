/*
 * A323455  Irregular triangle read by rows: row n lists the numbers that can
 *          be obtained from the binary expansion of n by inserting a single 0
 *          after any 1.
 * https://oeis.org/A323455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 448, u16 n <= 9718, u32 n <= 9718, u64 n <= 9718,
 *            u128 n <= 9718, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A323455.
 *
 * The triangle is defined as follows: for each integer r ≥ 1,
 * write r in binary.  Wherever a 1 occurs, we may insert an extra
 * 0 immediately after that 1, yielding a new binary string.  All such
 * strings are listed for row r, and the overall sequence is the row‑wise
 * reading of the triangle.  This routine finds the row containing the
 * n‑th term and reconstructs the appropriate value. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Cumulative counts of terms up to each row.  cum[0] = 0.
   cum[r] = total number of terms in rows 1..r. */
static idx_t *cum = NULL;
static size_t cum_len = 0;   /* number of rows currently stored */
static size_t cum_cap = 0;   /* allocated capacity of cum */

static void
extend_to_cover(idx_t target)
{
    if (cum_cap == 0) {
        cum_cap = 16;
        cum = (idx_t *)calloc(cum_cap, sizeof(idx_t));
        assert(cum != NULL);
        cum[0] = 0;
        cum_len = 0;
    }

    while (cum_len == 0 || cum[cum_len] < target) {
        if (cum_len + 1 >= cum_cap) {
            size_t new_cap = cum_cap * 2;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            assert(tmp != NULL);
            cum = tmp;
            cum_cap = new_cap;
        }
        /* Build next row, r = cum_len + 1 */
        idx_t r = (idx_t)(cum_len + 1);
        idx_t pop = __builtin_popcountll((unsigned long long)r);
        cum_len++;
        cum[cum_len] = cum[cum_len - 1] + pop;
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t
a(idx_t n)
{
    /* Make sure we know which row contains the n‑th term. */
    extend_to_cover(n);

    /* Binary search for row r such that cum[r‑1] < n ≤ cum[r]. */
    size_t low = 1, high = cum_len;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    size_t row_idx = low;                /* row number (1‑based) */
    idx_t row = (idx_t)row_idx;          /* value of the row */

    /* Position of the term within this row. */
    idx_t prev_sum = cum[row_idx - 1];
    idx_t pos = n - prev_sum;            /* 1‑based: 1 … popcount(row) */

    /* Reconstruct the number by inserting a 0 after the pos-th 1. */
    seq_t ans = 0;
    int ones_seen = 0;
    int bitlen = (int)(64 - __builtin_clzll((unsigned long long)row));

    for (int i = bitlen - 1; i >= 0; --i) {
        ans <<= 1;
        if ((row >> i) & 1ULL) {
            ans |= 1;
            ones_seen++;
            if ((idx_t)ones_seen == pos) {
                ans <<= 1;          /* insert the zero */
            }
        }
    }
    return ans;
}
