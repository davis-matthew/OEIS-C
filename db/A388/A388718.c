/*
 * A388718  Triangle read by rows: T(n,k) is the number of binary strings of
 *          length n which are not all zero and whose shortest run of 1's is
 *          of length k.
 * https://oeis.org/A388718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangular number a(n) for OEIS A388718.
 *  T(n,k) = U(n,k) – U(n,k+1)
 *  where U(n,k) = number of binary strings of length n (including the all‑zero
 *  string) in which every run of 1’s has length at least k.
 *
 *  U is defined recursively:
 *      U(0,k) = 1
 *      U(n,k) = U(n‑1,k) + (n ≥ k ? 1 : 0) + ∑_{m=0}^{\,n‑k‑1} U(m,k)
 *
 *  The value a(n) is obtained by reading the triangle row by row, with
 *  offset 1; i.e. a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), ….
 *  No external libraries are required beyond the standard headers below.   */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The precomputation is performed only once. */
    #define MAX_ROW 500              /* sufficient for n up to ~10 000 */
    static bool ready = false;
    static seq_t U[MAX_ROW+2][MAX_ROW+1];   /* U[k][len]  (k≥1, len≥0) */
    static seq_t T[MAX_ROW+1][MAX_ROW+1];   /* T[n][k]   (n≥1, 1≤k≤n)  */

    if (!ready) {
        const seq_t one = (seq_t)1;          /* small integer literal */

        /* initialise U(k,0)=1 for all k */
        for (idx_t k = 1; k <= MAX_ROW+1; ++k)
            U[k][0] = one;

        /* compute U(n,k) for all k and n */
        for (idx_t k = 1; k <= MAX_ROW+1; ++k) {
            seq_t prefix[MAX_ROW+1];       /* prefix sums for this k */
            prefix[0] = U[k][0];
            for (idx_t n = 1; n <= MAX_ROW; ++n) {
                seq_t sum_pref = (n >= k+1) ? prefix[n-k-1] : (seq_t)0;
                seq_t add_k    = (n >= k)     ? one : (seq_t)0;
                seq_t val      = U[k][n-1] + add_k + sum_pref;
                U[k][n]        = val;
                prefix[n]      = prefix[n-1] + val;
            }
        }

        /* compute the triangle entries T(n,k)=U(n,k)-U(n,k+1)   */
        for (idx_t n = 1; n <= MAX_ROW; ++n) {
            for (idx_t k = 1; k <= n; ++k)
                T[n][k] = U[k][n] - U[k+1][n];
        }

        ready = true;
    }

    /* determine the row (n) and column (k) of the requested index   */
    idx_t row = 0;
    while (row * (row + 1) / 2 < n)  /* integer arithmetic only */
        ++row;

    idx_t prev  = row * (row - 1) / 2; /* total terms before this row */
    idx_t col   = n - prev;            /* column index, 1‑based */

    return T[row][col];
}
