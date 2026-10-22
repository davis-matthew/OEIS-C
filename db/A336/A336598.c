/*
 * A336598  Triangle read by rows: T(n,k) is the number of linear chord
 *          diagrams on 2n vertices with one marked chord such that exactly k
 *          of the remaining n-1 chords cross the marked chord.
 * https://oeis.org/A336598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A336598:  Triangle read by rows, T(n,k) satisfies
 *  T(n,k) = n*T(n-1,k) + n*T(n-1,k-1)   (k >= 1)
 *  and  Σ_{k=0}^{n-1} T(n,k) = n*(2n-1)!!        (total matchings with one marked chord).
 *  The sequence a(n) is the flattened triangle (row‑major order).
 *  a(n) = T(r,c)   where  r = min { t : t(t+1)/2 >= n }
 *           c = n - (r*(r-1)/2 + 1)   (0‑based column index).
 *
 *  The implementation builds the triangle up to the required row
 *  using only integer arithmetic (no floating point); it maintains
 *  linear recurrence and uses the factorial identity for the total
 *  number of diagrams.
 *
 *  All arithmetic is performed in type seq_t; this works for
 *  8/16/32/64‑bit unsigned integers or for arbitrary‑precision
 *  integer classes (the same code compiles under C++17).
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Utility to compute the starting index of row r in the flat array.  */
static inline idx_t triangular_start(idx_t r) {
    return (idx_t)(((__int128)r * (r - 1)) / 2);
}

/* Find the row such that r*(r+1)/2 >= n.  */
static idx_t row_for_index(idx_t n) {
    if (n <= 1)
        return 1;
    double approx = (sqrt(1.0 + 8.0 * (double)n) - 1.0) / 2.0;
    idx_t r = (idx_t)approx;
    /* Adjust for possible rounding errors. */
    while (((__int128)r * (r + 1)) / 2 < n)
        ++r;
    while (r > 0 && (((__int128)(r - 1) * r) / 2) >= n)
        --r;
    return r;
}

/* Dynamic storage for the triangle and the odd double factorial values.  */
static seq_t *triangle = NULL;      /* Flat array: T(n,k) is at triangle[triangular_start(n)+k] */
static idx_t triangle_rows = 0;     /* Number of rows currently computed.  */
static seq_t *df = NULL;           /* df[i] = (2i-1)!!, length = df_rows+1  */
static idx_t df_rows = 0;          /* Largest i for which df[i] is defined.  */

/* Ensure that rows 1..needed are computed.  */
static void ensure_rows(idx_t needed) {
    if (needed <= triangle_rows)
        return;

    /* Allocate or grow the triangle storage.  */
    idx_t new_len = (needed * (needed + 1)) / 2;
    if (triangle == NULL) {
        triangle = (seq_t*)malloc(sizeof(seq_t) * new_len);
    } else {
        triangle = (seq_t*)realloc(triangle, sizeof(seq_t) * new_len);
    }

    /* Allocate or grow the double‑factorial storage.  */
    if (df == NULL) {
        df = (seq_t*)malloc(sizeof(seq_t) * (needed + 1));
        /* initialise base cases  */
        df[0] = 1;          /* (-1)!! = 1  */
        if (needed >= 1) {
            df[1] = 1;      /* 1!! = 1     */
            df_rows = 1;
        }
    } else if (needed > df_rows) {
        df = (seq_t*)realloc(df, sizeof(seq_t) * (needed + 1));
    }

    /* Compute new rows one by one.  */
    for (idx_t n = triangle_rows + 1; n <= needed; ++n) {
        /* Ensure df[n] is available.  */
        if (n > df_rows) {
            df[n] = df[n - 1] * (seq_t)(2 * n - 1);
            df_rows = n;
        }

        idx_t start_cur  = triangular_start(n);
        idx_t start_prev = triangular_start(n - 1);
        seq_t sum_k = 0;

        /* Compute T(n,k) for k = 1 .. n-1.  */
        for (idx_t k = 1; k <= n - 1; ++k) {
            seq_t val_prev_k  = (k <= n - 2) ? triangle[start_prev + k] : 0;
            seq_t val_prev_k1 = (k - 1 <= n - 2) ? triangle[start_prev + (k - 1)] : 0;
            seq_t val = (seq_t)n * val_prev_k + (seq_t)n * val_prev_k1;
            triangle[start_cur + k] = val;
            sum_k += val;
        }

        /* Sum of all entries in row n:  n * (2n-1)!!  */
        seq_t total = (seq_t)n * df[n];
        seq_t a0 = total - sum_k;              /* T(n,0) */
        triangle[start_cur] = a0;
    }

    triangle_rows = needed;
}

/* Public function required by the tester.  */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;  /* undefined, but safe to return 0 */

    idx_t r = row_for_index(n);                  /* row number (1‑based) */
    idx_t c = n - (triangular_start(r) + 1);     /* column index (0‑based) */
    ensure_rows(r);                               /* make sure up to row r is ready */
    return triangle[triangular_start(r) + c];
}
