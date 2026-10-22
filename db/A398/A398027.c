/*
 * A398027  Number of fixed 6-convex polyominoes with perimeter 2*n.
 * https://oeis.org/A398027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of fixed 6-convex polyominoes with perimeter 2*n.
 *            Computation via the generating function decomposition
 *            G(x)=R1(x)+R2(x)*(1-4x)^(-3/2) where R1,R2 are rational
 *            functions with small degree. The coefficients are obtained by
 *            solving linear recurrences for R1 and the proper part of R2,
 *            then co...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- Local static cache --------- */
    static seq_t *a1 = NULL;      /* coefficients of R1 part            */
    static seq_t *a2 = NULL;      /* coefficients of proper R2 part     */
    static seq_t *S  = NULL;      /* coefficients of (1-4x)^(-3/2)      */
    static seq_t *conv = NULL;    /* convolution of a2 and S            */
    static size_t cap = 0;        /* current allocated size              */
    static size_t cur = 0;        /* last computed index (inclusive)    */
    
    /* ---------- Helper to enlarge cache ----------
       We double the capacity each time we need more space.      */
    if (n >= (idx_t)cap) {
        size_t newcap = cap ? cap*2 : 16;
        while (newcap <= (size_t)n) newcap *= 2;
        a1   = (seq_t*)realloc(a1,   newcap * sizeof(seq_t));
        a2   = (seq_t*)realloc(a2,   newcap * sizeof(seq_t));
        S    = (seq_t*)realloc(S,    newcap * sizeof(seq_t));
        conv = (seq_t*)realloc(conv, newcap * sizeof(seq_t));
        cap = newcap;
    }
    
    /* ---------- Coefficient tables for rational parts ----------
       D1(x) = (1-4x)^2 * (x^3-6x^2+5x-1) * (7x^3-14x^2+7x-1)
         has degree 8.  Its coefficients d1[0..8] are:
           1,-20, 167,-752, 1966,-2984, 2471,-952, 112
       N1(x) = -x^2*(20x^9-127x^8+524x^7-1292x^6+1712x^5-1267x^4+544x^3-135x^2+18x-1)
         has degree 11.  Its coefficients n1[0..11] are:
           0, 0,  1,-18,135,-544,1267,-1712,1292,-524,127,-20
       
       D2(x) = (x^3-6x^2+5x-1)*(7x^3-14x^2+7x-1)
         has degree 6.  Its coefficients d2[0..6] are:
           1,-12,55,-120,126,-56,7
       N2(x) = -(1-2x)^2*(x^2-4x+1)^2*x^4
         computes to degree 10.  Its coefficients n2[0..10] are:
           0,0,0,0,-1,12,-54,112,-105,36,-4
    */
    static const int d1[9] = {1,-20, 167,-752, 1966,-2984, 2471,-952, 112};
    static const int n1[12] = {0, 0,  1,-18,135,-544,1267,-1712,1292,-524,127,-20};
    static const int d2[7] = {1,-12,55,-120,126,-56,7};
    static const int n2[11] = {0,0,0,0,-1,12,-54,112,-105,36,-4};
    
    /* ---------- Extend computation up to n ---------- */
    for (size_t i = cur; i <= (size_t)n; ++i) {
        /* ---- compute a1[i] ---- */
        seq_t sum1 = 0;
        for (int k = 1; k <= 8; ++k) {
            if (i >= (size_t)k)
                sum1 += d1[k] * a1[i - k];
        }
        seq_t rhs1 = (i <= 11 ? n1[i] : 0);
        a1[i] = rhs1 - sum1;   /* d1[0] == 1 */

        /* ---- compute a2[i] (proper part) ---- */
        seq_t sum2 = 0;
        for (int k = 1; k <= 6; ++k) {
            if (i >= (size_t)k)
                sum2 += d2[k] * a2[i - k];
        }
        seq_t rhs2 = (i <= 10 ? n2[i] : 0);
        a2[i] = rhs2 - sum2;   /* d2[0] == 1 */

        /* ---- compute S[i] for (1-4x)^(-3/2) ---- */
        if (i == 0) {
            S[0] = 1;
        } else {
            seq_t num = S[i - 1] * ((seq_t)4 * (seq_t)i + 2);
            S[i] = num / (seq_t)i;   /* exact division */
        }

        /* ---- compute convolution conv[i] = Σ_{k=0..i} a2[k]*S[i-k] ---- */
        seq_t sumconv = 0;
        for (size_t k = 0; k <= i; ++k) {
            sumconv += a2[k] * S[i - k];
        }
        conv[i] = sumconv;
    }
    cur = (size_t)n + 1;

    /* ---------- Final result ---------- */
    return a1[n] + conv[n];
}
