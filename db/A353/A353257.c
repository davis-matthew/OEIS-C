/*
 * A353257  Expansion of Sum_{k>=0} x^k * Product_{j=0..k-1} (2 * j - x).
 * https://oeis.org/A353257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * 2^(n-2*k) *
 *            |Stirling1(n-k,k)| (Stirling1 is the signed Stirling number of
 *            the first kind.) The implementation rewrites the absolute value
 *            using the sign of Stirling1 so that only signed Stirling numbers
 *            are stored. It precomputes all Stirling numbers s(m,k)...
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t N = n;                /* local copy of the requested index   */
    size_t size = (size_t)N + 1;/* array size (0 .. N)                 */

    /* allocation of helper arrays, compatible with C and C++ */
#ifdef __cplusplus
    seq_t* aArr   = new seq_t[size];      /* a[0..N]                           */
    seq_t* pow2   = new seq_t[size];      /* pow2[e] = 2^e, 0 <= e <= N        */
    seq_t* sPrev  = new seq_t[size];      /* Stirling numbers for row m-1      */
    seq_t* sCurr  = new seq_t[size];      /* Stirling numbers for row m        */
#else
    seq_t* aArr   = calloc(size, sizeof(seq_t));
    seq_t* pow2   = calloc(size, sizeof(seq_t));
    seq_t* sPrev  = calloc(size, sizeof(seq_t));
    seq_t* sCurr  = calloc(size, sizeof(seq_t));
#endif

    /* pre‑compute powers of two */
    pow2[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        pow2[i] = pow2[i-1] * ((seq_t)2);
    }

    /* initialise Stirling numbers for m = 0 : s(0,0) = 1 */
    sPrev[0] = (seq_t)1;      /* all other entries are zero by calloc/new */

    /* dp over m = 0 .. N, accumulating a[n] for all n <= N */
    for (idx_t m = 0; m <= N; ++m) {
        /* ensure that sPrev[m] is zero before using it */
        if (m > 0) {
            sPrev[m] = (seq_t)0;
        }

        /* compute row m of Stirling numbers s(m,k) */
        if (m == 0) {
            sCurr[0] = (seq_t)1;
        } else {
            sCurr[0] = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = sPrev[k-1] - ((seq_t)(m-1)) * sPrev[k];
                sCurr[k] = term;
            }
        }

        /* accumulate contributions to a[n] (n = m + k) */
        int signOdd = (m & 1);
        for (idx_t k = 0; k <= m; ++k) {
            idx_t idx = m + k;
            if (idx > N) break;
            if (sCurr[k] == (seq_t)0) continue;     /* skip zero terms   */
            seq_t val = sCurr[k] * pow2[m - k];
            if (signOdd) val = -val;
            aArr[idx] = aArr[idx] + val;
        }

        /* swap sPrev and sCurr for the next iteration */
        seq_t* tmp = sPrev;
        sPrev = sCurr;
        sCurr = tmp;
    }

    seq_t result = aArr[N];

    /* free the allocated memory */
#ifdef __cplusplus
    delete[] aArr;
    delete[] pow2;
    delete[] sPrev;
    delete[] sCurr;
#else
    free(aArr);
    free(pow2);
    free(sPrev);
    free(sCurr);
#endif

    return result;
}
