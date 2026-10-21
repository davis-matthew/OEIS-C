/*
 * A289925  p-INVERT of the lower Wythoff sequence (A000201), where p(S) = 1 -
 *          S - S^2.
 * https://oeis.org/A289925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A289925 : p‑INVERT of the lower Wythoff sequence
 *  a(n) = coefficient of x^(n+1) in 1/(1 - S(x) - S(x)^2) - 1
 *  where S(x) = Σ_{k≥1} floor(k·φ) · x^k        (φ = (1+√5)/2)
 *
 *  We compute the series up to the needed order using only integer
 *  arithmetic.  All arrays are grown as required and memoised between
 *  calls.  The implementation works for built‑in unsigned integer
 *  types and for arbitrary‑precision seq_t provided by the build.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: integer square root of a 64‑bit unsigned integer. */
static uint64_t _isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Helper: lower Wythoff number floor(k·φ).  Uses the formula
 *  w_k = (k + floor(√(5·k²))) / 2 .
 */
static seq_t _lower_wythoff(uint64_t k)
{
    /* Use 128‑bit to avoid overflow of 5*k*k. */
    uint64_t sq = (uint64_t)_isqrt_u64((unsigned __int128)5 * k * k);
    return (seq_t)((k + sq) >> 1);
}

/* Main sequence function.  All arrays are allocated lazily and memoised. */
seq_t a(idx_t n)
{
    /* We need coefficients up to index n+1 (inclusive). */
    static idx_t  max_req = 0;      /* maximum index that has been requested */
    static seq_t *S   = NULL;       /* lower Wythoff numbers, 0‑based, S[0]=0 */
    static seq_t *B   = NULL;       /* B = S + S^2 , 0‑based , B[0]=0 */
    static seq_t *A   = NULL;       /* A = 1/(1-B) , 0‑based , A[0]=1 */
    static size_t  cap = 0;         /* current length of allocated arrays */

    idx_t need = n + 2;                 /* need indices 0..n+1  -> size n+2 */

    if (cap < need) {
        /* enlarge: double size or at least reach needed */
        size_t newcap = cap > 0 ? cap : 8;
        while (newcap < need)
            newcap <<= 1;

        seq_t *news = (seq_t*)realloc(S, newcap * sizeof(seq_t));
        seq_t *newb = (seq_t*)realloc(B, newcap * sizeof(seq_t));
        seq_t *newa = (seq_t*)realloc(A, newcap * sizeof(seq_t));
        if (!news || !newb || !newa) { /* allocation failure */
            exit(EXIT_FAILURE);
        }
        S = news;  B = newb;  A = newa;

        /* initialise base terms if this is the first allocation */
        if (cap == 0) {
            S[0] = (seq_t)0;
            B[0] = (seq_t)0;
            A[0] = (seq_t)1;
        }

        /* compute new entries from cap up to newcap-1 */
        for (idx_t i = cap; i < newcap; ++i) {
            if (i == 0) continue;              /* base already set */

            /* S[i] = lower Wythoff number for i (i ≥ 1) */
            S[i] = _lower_wythoff((uint64_t)i);

            /* conv = (S * S)[i] = sum_{j=1}^{i-1} S[j] * S[i-j] */
            seq_t conv = (seq_t)0;
            for (idx_t j = 1; j < i; ++j)
                conv += S[j] * S[i - j];

            B[i] = S[i] + conv;                 /* B = S + S^2 */

            /* A[i] = Σ_{k=1..i} B[k] * A[i-k]        (since A = 1 + B*A) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k)
                sum += B[k] * A[i - k];
            A[i] = sum;
        }

        cap = newcap;
    }

    /* a(n) is the coefficient of x^(n+1) in A(x)-1, which equals A[n+1]. */
    return A[n + 1];
}
