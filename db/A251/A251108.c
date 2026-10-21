/*
 * A251108  Number of (n+1)X(3+1) 0..2 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251108
   a(n) = number of (n+1)×4 arrays over {0,1,2} with no 2×2 subblock
   having max(diagonal) > |difference of antidiagonal|.
   Computed by adjacency‑matrix exponentiation over all 81 possible
   row patterns. */

#include <stddef.h>          /* for size_t */

/* Number of row patterns (3^4). 81 × 81 = 6561 entries. */
#define PAT 81

/* --------------------------------------------------------------------- */
/* Helper: multiply two PAT×PAT matrices.  Result is stored in C.       */
static void mul_mat(const seq_t (*A)[PAT], const seq_t (*B)[PAT],
                    seq_t (*C)[PAT])
{
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            C[i][j] = (seq_t)0;

    for (int i = 0; i < PAT; ++i)
        for (int k = 0; k < PAT; ++k) {
            const seq_t aik = A[i][k];
            if (aik == (seq_t)0) continue;
            for (int j = 0; j < PAT; ++j) {
                const seq_t bkj = B[k][j];
                if (bkj == (seq_t)0) continue;
                C[i][j] += aik * bkj;
            }
        }
}
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------- */
    /* Initialise transition matrix once per program run.               */
    /* ----------------------------------------------------------------- */
    static int init = 0;
    static seq_t M[PAT][PAT];
    static int pat[PAT][4];

    if (!init) {
        /* Encode all 81 row patterns (base‑3 representation). */
        for (int i = 0; i < PAT; ++i) {
            int x = i;
            for (int c = 3; c >= 0; --c) {
                pat[i][c] = x % 3;
                x /= 3;
            }
        }

        /* Build adjacency matrix M: M[i][j] = 1 if rows i→j satisfy the
           2×2 block restriction, otherwise 0. */
        for (int i = 0; i < PAT; ++i) {
            for (int j = 0; j < PAT; ++j) {
                int ok = 1;
                for (int col = 0; col < 3; ++col) {
                    int d1    = pat[i][col];
                    int d2    = pat[j][col + 1];
                    int maxd  = d1 > d2 ? d1 : d2;
                    int ant1  = pat[i][col + 1];
                    int ant2  = pat[j][col];
                    int diff  = ant1 > ant2 ? ant1 - ant2 : ant2 - ant1;
                    if (maxd > diff) {
                        ok = 0;
                        break;
                    }
                }
                M[i][j] = ok ? (seq_t)1 : (seq_t)0;
            }
        }
        init = 1;
    }

    /* ----------------------------------------------------------------- */
    /* Prepare temporary matrices for exponentiation.                    */
    /* ----------------------------------------------------------------- */
    seq_t base[PAT][PAT];
    seq_t P[PAT][PAT];
    seq_t T[PAT][PAT];
    seq_t S[PAT][PAT];

    /* base = M (copy) */
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            base[i][j] = M[i][j];

    /* P = identity matrix */
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            P[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* Fast exponentiation of base to the power n. */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mul_mat(P, base, T);   /* P = P * base */
            for (int i = 0; i < PAT; ++i)
                for (int j = 0; j < PAT; ++j)
                    P[i][j] = T[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        mul_mat(base, base, S);   /* base = base * base */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                base[i][j] = S[i][j];
    }

    /* ----------------------------------------------------------------- */
    /* Sum all entries of P = M^n to obtain the total number of arrays.  */
    /* ----------------------------------------------------------------- */
    seq_t total = (seq_t)0;
    for (int i = 0; i < PAT; ++i)
        for (int j = 0; j < PAT; ++j)
            total += P[i][j];

    return total;
}
