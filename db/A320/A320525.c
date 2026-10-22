/*
 * A320525  Triangle read by rows: T(n,k) = number of chiral pairs of color
 *          patterns (set partitions) in a row of length n using exactly k
 *          colors (subsets).
 * https://oeis.org/A320525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = (S₂(n,k) – A(n,k))/2 where S₂ is Stirling numbers of
 *            the second kind, A(n,k) follows: A(n,k) = k*A(n-2,k) +
 *            A(n-2,k-1) + A(n-2,k-2) for n>1 A(0,0)=1, A(1,1)=1, all other
 *            A(0,k)=A(1,k)=0 The sequence a(n) is the triangle read by rows
 *            (row r contains T(r,1)...T(r,r)), indexed from 1
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 140, u64 n <= 358,
 *            u128 n <= 957, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the triangle row by row,
   storing only the current and the two previous rows
   of A, and the previous row of S.  The flat sequence
   a(n) is accumulated in a dynamic array for O(1)
   retrieval of any index. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Static state for incremental construction of the triangle         */
    /* ------------------------------------------------------------------ */
    static bool initialized = false;
    static seq_t *a_seq      = NULL;  /* flat sequence a(1),a(2),...  */
    static idx_t a_seq_len   = 1;      /* number of elements in a_seq   */
    static idx_t a_seq_cap   = 1;      /* current capacity of a_seq     */

    static seq_t *S_prev = NULL;       /* S₂ for previous row (n-1)    */
    static idx_t S_prev_len = 0;       /* length of S_prev              */

    static seq_t *A_prev2 = NULL;      /* A row for n-2                 */
    static idx_t A_prev2_len = 0;       /* length of A_prev2             */
    static seq_t *A_prev1 = NULL;      /* A row for n-1                 */
    static idx_t A_prev1_len = 0;       /* length of A_prev1             */

    static idx_t current_row = 0;      /* rows already processed (0 means none) */

    /* ------------------------------------------------------------------ */
    /* One-time initialization                                          */
    /* ------------------------------------------------------------------ */
    if (!initialized) {
        /* sequence array: index 0 unused, filled with dummy */
        a_seq = (seq_t*)malloc(sizeof(seq_t));
        a_seq[0] = (seq_t)0;   /* dummy value */
        a_seq_len = 1;
        a_seq_cap = 1;

        /* S₂ row 0: S₂(0,0)=1 */
        S_prev = (seq_t*)malloc(sizeof(seq_t));
        S_prev[0] = (seq_t)1;
        S_prev_len = 1;

        /* A row 0: A(0,0)=1 */
        A_prev2 = (seq_t*)malloc(sizeof(seq_t));
        A_prev2[0] = (seq_t)1;
        A_prev2_len = 1;

        /* A row 1 will be created during the first iteration */
        A_prev1 = NULL;
        A_prev1_len = 0;

        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* Grow the triangle until we have a_seq[n] available                */
    /* ------------------------------------------------------------------ */
    while (a_seq_len <= n) {
        /* ----- compute next row (row = current_row + 1) ----- */
        idx_t row = current_row + 1;
        current_row = row;

        /* --- allocate arrays for the new row --- */
        /* A_cur will have indices 0..row (row+1 elements) */
        seq_t *A_cur = (seq_t*)calloc((size_t)(row + 1), sizeof(seq_t));
        /* S_cur similarly */
        seq_t *S_cur = (seq_t*)calloc((size_t)(row + 1), sizeof(seq_t));

        /* ----- compute A_cur for this row ----- */
        if (row == 1) {
            /* Base case: A(1,1) = 1 */
            A_cur[1] = (seq_t)1;
        } else {
            /* Recurrence using A_prev2 (row-2) */
            for (idx_t k = 1; k <= row; ++k) {
                seq_t term1 = (k     < A_prev2_len) ? A_prev2[k]     : (seq_t)0;
                seq_t term2 = (k - 1 < A_prev2_len) ? A_prev2[k-1]   : (seq_t)0;
                seq_t term3 = (k >= 2 && k-2 < A_prev2_len) ? A_prev2[k-2] : (seq_t)0;
                A_cur[k] = (seq_t)k * term1 + term2 + term3;
            }
        }

        /* ----- compute S_cur for this row (Stirling S₂) ----- */
        for (idx_t k = 1; k <= row; ++k) {
            seq_t term1 = (k     < S_prev_len) ? S_prev[k]     : (seq_t)0;
            seq_t term2 = (k - 1 < S_prev_len) ? S_prev[k-1]   : (seq_t)0;
            S_cur[k] = (seq_t)k * term1 + term2;
        }

        /* ----- compute T(n,k) and append to a_seq ----- */
        for (idx_t k = 1; k <= row; ++k) {
            seq_t val = (S_cur[k] - A_cur[k]) / (seq_t)2;
            if (a_seq_len == a_seq_cap) {
                idx_t new_cap = (a_seq_cap == 0) ? 1 : a_seq_cap * 2;
                seq_t *tmp = (seq_t*)realloc(a_seq, (size_t)new_cap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failure */
                a_seq = tmp;
                a_seq_cap = new_cap;
            }
            a_seq[a_seq_len] = val;
            ++a_seq_len;
        }

        /* ----- shift S and A rows for next iteration ----- */
        free(S_prev);
        S_prev = S_cur;
        S_prev_len = row + 1;

        if (row == 1) {
            /* After row 1, keep A_prev2 (row 0) and set A_prev1 = row 1 */
            A_prev1 = A_cur;
            A_prev1_len = row + 1;
            /* A_prev2 remains as row 0 */
        } else {
            /* For rows >= 2: discard old A_prev2, shift pointers */
            free(A_prev2);
            A_prev2 = A_prev1;
            A_prev2_len = A_prev1_len;
            A_prev1 = A_cur;
            A_prev1_len = row + 1;
        }
    }

    /* ------------------------------------------------------------------ */
    /* Return the requested value                                         */
    /* ------------------------------------------------------------------ */
    return a_seq[n];
}
