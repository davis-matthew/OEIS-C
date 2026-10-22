/*
 * A394150  G.f. A(x) satisfies
 *          A(x)^6-2*A(x)^5+(6x+1)*A(x)^4-8x*A(x)^3+(9x^2+2x)*A(x)^2-5x^2*A(x)+3x^3=0.
 * https://oeis.org/A394150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   OEIS A394150 – Number of n‑vertex planar rooted trees with vertices
   colored red, blue, and green (green root), with the specified adjacency
   constraints.
   The function computes the sequence a(n) defined by the generating
   function equation
     A^6 - 2*A^5 + (6x+1)*A^4 - 8x*A^3 + (9x^2+2x)*A^2 - 5x^2*A + 3x^3 = 0,
   where a(0)=0, a(1)=1, a(2)=2, a(3)=8, ... .
   Computation is done by keeping the generating series of the three
   colour classes (Green, Red, Blue) and their associated
   "geometric series" sums.  The recurrence is O(n²) and fits easily
   within the time limit for n up to about 10000.
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* {seq_t} and {idx_t} are supplied by the build. */
seq_t a(idx_t n) {
    /* Special handling of the trivial index 0. */
    if (n == 0) return (seq_t)0;

    /* Static storage for the series coefficients. */
    static seq_t *G = NULL, *R = NULL, *B = NULL;
    static seq_t *S_RG = NULL, *S_RB = NULL, *S_RGB = NULL;
    static idx_t size = 0;          /* current allocated length */
    static idx_t last = 0;          /* highest index for which G, R, B are known */

    /* Ensure we have enough space for indices up to n. */
    if (size <= n) {
        idx_t newsize = (idx_t)(n + 1);
        G =       (seq_t *)realloc(G,       newsize * sizeof(seq_t));
        R =       (seq_t *)realloc(R,       newsize * sizeof(seq_t));
        B =       (seq_t *)realloc(B,       newsize * sizeof(seq_t));
        S_RG =    (seq_t *)realloc(S_RG,    newsize * sizeof(seq_t));
        S_RB =    (seq_t *)realloc(S_RB,    newsize * sizeof(seq_t));
        S_RGB =   (seq_t *)realloc(S_RGB,   newsize * sizeof(seq_t));

        /* Zero new parts */
        if (size > 0) {
            memset(G + size,       0, (newsize - size) * sizeof(seq_t));
            memset(R + size,       0, (newsize - size) * sizeof(seq_t));
            memset(B + size,       0, (newsize - size) * sizeof(seq_t));
            memset(S_RG + size,    0, (newsize - size) * sizeof(seq_t));
            memset(S_RB + size,    0, (newsize - size) * sizeof(seq_t));
            memset(S_RGB + size,   0, (newsize - size) * sizeof(seq_t));
        }

        /* Initial conditions (they are required before any recursion). */
        if (size == 0) {
            /* Base values for the geometric series. */
            S_RG[0]  = 1;
            S_RB[0]  = 1;
            S_RGB[0] = 1;
            /* The one‑vertex trees for each colour.                   */
            G[1] = 1;  /* green root only                         */
            R[1] = 1;  /* red root only                           */
            B[1] = 1;  /* blue root only                          */
            last = 1;  /* we have computed up to index 1 so far      */
        }
        size = newsize;
    }

    /* The result a( n ) is G[n].  Compute terms up to n if necessary. */
    for (idx_t m = last; m < n; ++m) {
        /* Compute S_RG[m] = Σ_{k=1..m} (R[k] + G[k]) * S_RG[m-k] */
        seq_t sum_rg = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum_rg += (R[k] + G[k]) * S_RG[m - k];
        }
        S_RG[m] = sum_rg;

        /* Compute S_RB[m] = Σ_{k=1..m} (R[k] + B[k]) * S_RB[m-k] */
        seq_t sum_rb = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum_rb += (R[k] + B[k]) * S_RB[m - k];
        }
        S_RB[m] = sum_rb;

        /* Compute S_RGB[m] = Σ_{k=1..m} (R[k] + G[k] + B[k]) * S_RGB[m-k] */
        seq_t sum_rgb = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum_rgb += (R[k] + G[k] + B[k]) * S_RGB[m - k];
        }
        S_RGB[m] = sum_rgb;

        /* Translate the geometric series into coefficients of the colours. */
        G[m + 1] = S_RG[m];
        R[m + 1] = S_RB[m];
        B[m + 1] = S_RGB[m];
    }

    last = n;              /* we now have all indices up to n ready */
    return G[n];
}
