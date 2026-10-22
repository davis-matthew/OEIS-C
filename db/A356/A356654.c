/*
 * A356654  Triangle read by rows. T(n, k) = k! * Sum_{j=k..n} Lah(n, j) *
 *          Stirling2(j, k), where Lah(n, k) = A271703(n, k).
 * https://oeis.org/A356654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened triangle T(n,k) with * T(n,k) = k! * ∑_{j=k..n}
 *            L(n,j) * S(j,k), * where L is the Lah number A271703(n,k) and S
 *            is Stirling2(n,k)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes terms of OEIS A356654.  It builds the
 * triangle up to the required row, memoizing the results in static
 * tables.  The implementation works for any seq_t, including
 * arbitrary‑precision C++ types (hence all realloc calls are cast). */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /*--- state for memoisation -------------------------------------------*/
    static idx_t computed_rows = 0;          /* number of rows already built     */
    static seq_t *fact = NULL;              /* factorial table, fact[i] = i!     */
    static seq_t **S = NULL;                /* Stirling2 table, S[r][k]          */
    static seq_t **L = NULL;                /* Lah numbers, L[r][k]              */
    static seq_t *seq = NULL;               /* flattened triangle values         */
    static size_t seq_len  = 0;              /* number of entries stored          */
    static size_t seq_cap  = 0;              /* capacity of seq                   */

    /*--- find the row that contains index n -----------------------------*/
    idx_t row = 0;
    while ((idx_t)(row + 1) * (row + 2) / 2 <= n)
        ++row;                              /* row is now the correct one       */
    idx_t col = n - row * (row + 1) / 2;    /* unused but harmless              */

    /*--- build rows up to 'row' (inclusive) if needed -------------------*/
    while (computed_rows <= row) {
        idx_t r = computed_rows;              /* next row to compute              */

        /*--- factorials ---------------------------------------------------*/
        fact = (seq_t *)realloc(fact, (size_t)(r + 1) * sizeof(seq_t));
        if (!fact) return 0;                 /* allocation failed                 */
        if (r == 0)
            fact[0] = (seq_t)1;
        else
            fact[r] = fact[r - 1] * (seq_t)r;

        /*--- allocate arrays for this row ---------------------------------*/
        seq_t *S_row = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        seq_t *L_row = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
        if (!S_row || !L_row) return 0;     /* allocation failed                 */

        /*--- Stirling numbers of the second kind S[r][k] ------------------*/
        if (r == 0) {
            S_row[0] = (seq_t)1;
        } else {
            S_row[0] = (seq_t)0;
            for (idx_t k = 1; k <= r; ++k) {
                seq_t t1 = (k <= r - 1) ? k * S[r - 1][k] : (seq_t)0;
                seq_t t2 = (k - 1 <= r - 1) ? S[r - 1][k - 1] : (seq_t)0;
                S_row[k] = t1 + t2;
            }
        }

        /*--- Lah numbers L[r][k] ----------------------------------------*/
        if (r == 0) {
            L_row[0] = (seq_t)1;
        } else {
            L_row[0] = (seq_t)0;
            for (idx_t k = 1; k <= r; ++k) {
                seq_t term1 = (seq_t)1 * L[r - 1][k - 1];   /* L(r-1,k-1)   */
                seq_t term2 = (k <= r - 1)
                               ? ((seq_t)(r + k - 1)) * L[r - 1][k]
                               : (seq_t)0;
                L_row[k] = term1 + term2;
            }
        }

        /*--- store the new row pointers ----------------------------------*/
        S = (seq_t **)realloc(S, (size_t)(r + 1) * sizeof(seq_t *));
        L = (seq_t **)realloc(L, (size_t)(r + 1) * sizeof(seq_t *));
        if (!S || !L) return 0;               /* allocation failed                 */
        S[r] = S_row;
        L[r] = L_row;

        /*--- append the triangle values to the flattened array ----------*/
        size_t needed = seq_len + (r + 1);
        if (needed > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
            while (new_cap < needed) new_cap *= 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!seq) return 0;               /* allocation failed                 */
            seq_cap = new_cap;
        }

        if (r == 0) {
            seq[seq_len++] = (seq_t)1;         /* T(0,0) = 1 */
        } else {
            seq[seq_len++] = (seq_t)0;         /* T(r,0) = 0 for r>0 */
            for (idx_t k = 1; k <= r; ++k) {
                seq_t sum = (seq_t)0;
                for (idx_t j = k; j <= r; ++j) {
                    sum += L[r][j] * S[j][k];
                }
                seq[seq_len++] = fact[k] * sum;
            }
        }

        ++computed_rows;                      /* prepare for next row            */
    }

    /*--- return the requested entry -------------------------------------*/
    return seq[n];
}
