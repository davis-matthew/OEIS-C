/*
 * A249128  Triangular array: row n gives the coefficients of the polynomial
 *          p(n,x) defined in Comments.
 * https://oeis.org/A249128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 106, u32 n <= 278, u64 n <= 744,
 *            u128 n <= 2084, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Recurrence:  f(0,x)=1 ,
 *  f(n,x) = x + floor((n+1)/2) / f(n-1,x)   (n≥1)
 *  p(n,x) = numerator of f(n,x)
 *  a(n)   = the nth flattened entry of the triangular array of coefficients of p(n,x)
 *
 *  The function below computes a(n) for any n≥0.
 *  It uses only integer arithmetic for indices and
 *  the provided seq_t type for the coefficients.
 */

#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
/* In C++ use new[] to construct objects of type seq_t */
#define SEQ_ALLOC(sz) new seq_t[(sz)]
#define SEQ_FREE(p)   delete [] (p)
#else
/* In C use malloc (no constructors are needed for POD types) */
#define SEQ_ALLOC(sz) ((seq_t*)malloc((sz) * sizeof(seq_t)))
#define SEQ_FREE(p)   free(p)
#endif

seq_t a(idx_t n)
{
    /* Find triangle row r and column c such that
     * r*(r+1)/2 <= n < (r+1)*(r+2)/2
     * c = n - r*(r+1)/2
     */
    idx_t r;
    if (n == 0) {
        r = 0;
    } else {
        long double dn = (long double)n;
        r = (idx_t)((sqrtl(1.0L + 8.0L*dn) - 1.0L) / 2.0L);
        /* Correct possible rounding errors */
        while (r*(r + 1)/2 > n) --r;
        while ((r + 1)*(r + 2)/2 <= n) ++r;
    }
    idx_t c = n - r*(r + 1)/2;           /* column index in row r */

    /* Special cases n=0 or n=1, no need to allocate arrays */
    if (r == 0) {
        return (seq_t)1;
    }
    if (r == 1) {
        /* Row 1: coefficients [1,1] */
        return (c == 0) ? (seq_t)1 : (seq_t)1;
    }

    /* Allocate space for three successive rows, maximum length is r+1 */
    size_t sz = (size_t)(r + 1);
    seq_t *P_pp = SEQ_ALLOC(sz);   /* row r-2  */
    seq_t *P_p  = SEQ_ALLOC(sz);   /* row r-1  */
    seq_t *P    = SEQ_ALLOC(sz);   /* row r    */

    if (!P_pp || !P_p || !P) {     /* allocation failure */
        if (P_pp) SEQ_FREE(P_pp);
        if (P_p)  SEQ_FREE(P_p);
        if (P)    SEQ_FREE(P);
        return (seq_t)0;            /* safe default */
    }

    /* Base rows */
    P_pp[0] = (seq_t)1;            /* p(0,x) = 1 */
    P_p[0]  = (seq_t)1;            /* p(1,x) constant term */
    P_p[1]  = (seq_t)1;            /* p(1,x) x‑coefficient */

    /* Compute rows 2 … r */
    for (idx_t row = 2; row <= r; ++row) {
        seq_t k = (seq_t)((row + 1) >> 1);   /* floor((row+1)/2) */

        /* Constant term */
        P[0] = (P_pp[0] * k);

        /* Remaining coefficients */
        for (idx_t i = 1; i <= row; ++i) {
            seq_t val = P_p[i - 1];
            if (i <= row - 2) {          /* avoid out of bounds */
                val += (P_pp[i] * k);
            }
            P[i] = val;
        }

        /* Rotate pointers for next iteration */
        seq_t *tmp = P_pp;
        P_pp = P_p;
        P_p  = P;
        P    = tmp;   /* now P holds the old P_pp, which will be reused */
    }

    /* P_p now holds the coefficients of row r */
    seq_t result = P_p[c];

    /* Free allocated memory */
    SEQ_FREE(P_pp);
    SEQ_FREE(P_p);
    SEQ_FREE(P);

    return result;
}
