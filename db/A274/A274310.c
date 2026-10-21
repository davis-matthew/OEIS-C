/*
 * A274310  Triangle read by rows: T(n,k) = number of parity alternating
 *          partitions of [n] into k blocks (1 <= k <= m).
 * https://oeis.org/A274310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) with T(r,k) = Sum_{i=0..k-1}
 *            Stirling2(floor((r+2)/2), i+1) * Stirling2(floor((r+1)/2), k-i)
 * Range:     u8 n <= 31, u16 n <= 82, u32 n <= 196, u64 n <= 476,
 *            u128 n <= 1292, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_mul.h"   /* for seq_mul(seq_t, seq_t) */

/* globals for memoising Stirling numbers of the second kind */
static idx_t S_max_n = 0;          /* highest n for which Stirling numbers have been computed */
static seq_t *S_table  = NULL;     /* flat array S[n][k] stored as S[n*width + k] */

/* ensure Stirling numbers of the second kind are available up to n = needed */
static void ensure_stirling(idx_t needed)
{
    if (needed <= S_max_n)
        return;

    idx_t new_max = needed;
    size_t width  = new_max + 1;
    size_t new_size = width * width;

    seq_t *new_table = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_table)
        abort();  /* cannot allocate memory */

    /* initialize all entries to zero */
    for (size_t i = 0; i < new_size; ++i)
        new_table[i] = (seq_t)0;

    /* copy existing values (if any) into the new table */
    if (S_table) {
        size_t old_width = S_max_n + 1;
        for (idx_t n = 0; n <= S_max_n; ++n) {
            for (idx_t k = 0; k <= n; ++k) {
                new_table[n*width + k] = S_table[n*old_width + k];
            }
        }
        free(S_table);
    }

    /* base case: S(0,0) = 1 */
    new_table[0] = (seq_t)1;

    /* compute new rows by recurrence */
    for (idx_t n = S_max_n + 1; n <= new_max; ++n) {
        new_table[n*width + 0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t left  = new_table[(n-1)*width + (k-1)];
            seq_t right = (k <= n-1) ?
                          seq_mul(new_table[(n-1)*width + k], (seq_t)k) :
                          (seq_t)0;
            new_table[n*width + k] = left + right;
        }
    }

    S_table  = new_table;
    S_max_n  = new_max;
}

/* compute T(row, col) as defined above */
static seq_t compute_T(idx_t row, idx_t col)
{
    idx_t a = (row + 2) / 2;          /* ⌊(row+2)/2⌋ */
    idx_t b = (row + 1) / 2;          /* ⌊(row+1)/2⌋ */
    idx_t needed = a > b ? a : b;
    ensure_stirling(needed);

    size_t width = S_max_n + 1;
    seq_t sum = (seq_t)0;

    for (idx_t i = 0; i < col; ++i) {
        idx_t i1 = i + 1;
        idx_t i2 = col - i;
        if (i1 > a || i2 > b)
            continue;
        seq_t term1 = S_table[a*width + i1];
        seq_t term2 = S_table[b*width + i2];
        seq_t prod  = seq_mul(term1, term2);
        sum += prod;
    }
    return sum;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, but return 0 */
        return (seq_t)0;

    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t prev = (r - 1) * r / 2;      /* total entries before row r */
    idx_t k    = n - prev;             /* position within the row (1‑based) */

    return compute_T(r, k);
}
