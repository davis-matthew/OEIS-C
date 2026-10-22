/*
 * A366030  Square array A(n, k), n, k > 0, read and filled by upwards
 *          antidiagonals the greedy way with distinct positive integers such
 *          that any two distinct terms in the same row or column or
 *          antidiagonal are coprime.
 * https://oeis.org/A366030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A366030 – square array A(n,k) filled by upwards
    antidiagonals with the greedy rule.
    The sequence a(n) is the order in which the array entries
    are filled.  No closed formula is known; the array is
    constructed locally, filling each cell with the smallest
    unused positive integer that is coprime to all numbers
    already in the same row, column or antidiagonal.
    The function is memoised, so calls may be in any order. */

#include <stddef.h>      /* size_t */
#include <stdbool.h>     /* bool */
#include "../utils/gcd.h"/* gcd(seq_t,seq_t) */

#define MAX_DIM   400           /* > sqrt(2*MAX_N) for n up to 1e4 */
#define MAX_TERMS 40000         /* enough for n=1e4 (triangular≈8k) */

static seq_t        arr[MAX_DIM + 1][MAX_DIM + 1]; /* zero‑initialised */
static seq_t        seq_values[MAX_TERMS];
static size_t       seq_len     = 0;                 /* number of values produced */
static idx_t        current_sum = 2;                 /* i + j of the current antidiagonal */
static idx_t        current_pos = 0;                 /* index within the current diagonal */

/*--- Check whether cand can be placed at (i,j) on antidiagonal with sum = i + j */
static bool
candidate_ok(seq_t cand, idx_t i, idx_t j, idx_t sum)
{
    /* 1. must be distinct from any previously used number */
    for (size_t t = 0; t < seq_len; ++t)
        if (seq_values[t] == cand)
            return false;

    /* 2. coprime with every number already in the same row i */
    for (idx_t col = 1; col <= sum - 1; ++col) {
        seq_t v = arr[i][col];
        if (v && gcd(v, cand) != (seq_t)1)
            return false;
    }

    /* 3. coprime with every number already in the same column j */
    for (idx_t row = 1; row <= sum - 1; ++row) {
        seq_t v = arr[row][j];
        if (v && gcd(v, cand) != (seq_t)1)
            return false;
    }

    /* 4. coprime with every number already on this antidiagonal   */
    for (idx_t row = 1; row <= sum - 1; ++row) {
        idx_t col = sum - row;                 /* cell belonging to this diagonal */
        seq_t v = arr[row][col];
        if (v && gcd(v, cand) != (seq_t)1)
            return false;
    }

    return true;
}

/*--- Return the n‑th term of A366030 (n >= 1) ---------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* safety for 0‑based callers */

    while (seq_len < n) {
        idx_t i = current_sum - 1 - current_pos; /* row index (1‑based)  */
        idx_t j = current_pos + 1;               /* column index (1‑based) */

        /* Find the smallest unused integer satisfying all constraints. */
        seq_t cand = 1;
        while (!candidate_ok(cand, i, j, current_sum))
            ++cand;

        /* Store the chosen value. */
        arr[i][j]          = cand;
        seq_values[seq_len] = cand;
        ++seq_len;

        /* Advance to the next cell along the current diagonal. */
        ++current_pos;
        if (current_pos == current_sum - 1) {
            current_pos = 0;
            ++current_sum;
        }
    }

    return seq_values[n - 1];
}
