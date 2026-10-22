/*
 * A320198  Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 10*d^2 = n.
 * https://oeis.org/A320198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 10*d^2 = n
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a,b,c,d) ∈ ℤ, multiplicity: 1 for zero, 2 for non‑zero (±) */

#include <stdlib.h>
#include <stdint.h>

/* integer square root for idx_t */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* maximum indices for each variable */
    const idx_t maxA = isqrt_u64(n);                 /* a^2 ≤ n          */
    const idx_t maxB = isqrt_u64(n / 2);             /* 2*b^2 ≤ n        */
    const idx_t maxC = isqrt_u64(n / 4);             /* 4*c^2 ≤ n        */
    const idx_t maxD = isqrt_u64(n / 10);            /* 10*d^2 ≤ n       */

    /* temporary arrays for successive convolutions.
       Size n+1 suffices because every partial sum is <= n. */
    seq_t *tmp1 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *tmp2 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *tmp3 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));

    if (!tmp1 || !tmp2 || !tmp3) {
        free(tmp1); free(tmp2); free(tmp3);
        return (seq_t)0;
    }

    /* Convolution of a and b */
    for (idx_t a0 = 0; a0 <= maxA; ++a0) {
        const idx_t valA = a0 * a0;
        const seq_t cntA = (a0 == 0) ? (seq_t)1 : (seq_t)2;
        for (idx_t b0 = 0; b0 <= maxB; ++b0) {
            const idx_t valB = 2 * b0 * b0;
            idx_t sumAB = valA + valB;
            if (sumAB > n) continue;
            const seq_t cntB = (b0 == 0) ? (seq_t)1 : (seq_t)2;
            tmp1[(size_t)sumAB] += cntA * cntB;
        }
    }

    /* Convolution of (a,b) with c */
    for (idx_t s = 0; s <= n; ++s) {
        if (tmp1[(size_t)s] == (seq_t)0) continue;
        const seq_t cntAB = tmp1[(size_t)s];
        for (idx_t c0 = 0; c0 <= maxC; ++c0) {
            const idx_t valC = 4 * c0 * c0;
            idx_t sumABC = s + valC;
            if (sumABC > n) continue;
            const seq_t cntC = (c0 == 0) ? (seq_t)1 : (seq_t)2;
            tmp2[(size_t)sumABC] += cntAB * cntC;
        }
    }

    /* Convolution of (a,b,c) with d */
    for (idx_t s = 0; s <= n; ++s) {
        if (tmp2[(size_t)s] == (seq_t)0) continue;
        const seq_t cntABC = tmp2[(size_t)s];
        for (idx_t d0 = 0; d0 <= maxD; ++d0) {
            const idx_t valD = 10 * d0 * d0;
            idx_t sumABCD = s + valD;
            if (sumABCD > n) continue;
            const seq_t cntD = (d0 == 0) ? (seq_t)1 : (seq_t)2;
            tmp3[(size_t)sumABCD] += cntABC * cntD;
        }
    }

    /* Result: number of representations summing to n */
    const seq_t result = tmp3[(size_t)n];

    free(tmp1); free(tmp2); free(tmp3);
    return result;
}
