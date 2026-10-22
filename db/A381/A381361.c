/*
 * A381361  E.g.f. satisfies A(x) = exp( Integral abs(1/A(x)^2) dx ), where
 *          abs(F(x)) equals the series expansion formed by the unsigned
 *          coefficients in F(x).
 * https://oeis.org/A381361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n+1)=∑_{k=0}^{n} binom(n,k)*a(k)*|h(n-k)| where
 *            h(0)=1, h(n)=−∑_{i=1}^{n} binom(n,i)*C(i)*h(n-i) and
 *            C(n)=∑_{i=0}^{n} binom(n,i)*a(i)*a(n-i). The recurrence follows
 *            from the differential‑equation A' = A·abs(1/A²) with
 *            A(x)=∑a(n)xⁿ/n!. All arithmetic is exact on seq_t
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   add_signed_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_signed_seq.h"

/* The sequence function. */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return 1;

    /* allocate required storage (indices 0 … n) */
    seq_t *a_arr  = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *C_arr  = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *magH   = (seq_t *)calloc(n + 1, sizeof(seq_t));
    bool  *negH   = (bool  *)calloc(n + 1, sizeof(bool ));
    if (!a_arr || !C_arr || !magH || !negH) {
        /* allocation failure – return 0 as a safe value */
        return 0;
    }

    /* initial values */
    a_arr[0]  = 1;           /* a(0) */
    C_arr[0]  = 1;           /* C(0) = a0*a0 */
    magH[0]   = 1;           /* |h(0)| = 1 */
    negH[0]   = false;       /* h(0) positive */

    for (idx_t nnow = 0; nnow < n; ++nnow) {
        /* ===== 1. Compute C(nnow) for nnow>0 ===== */
        if (nnow > 0) {
            seq_t sumC = 0;
            seq_t bin  = 1;   /* binom(nnow,0) */
            for (idx_t i = 0; i <= nnow; ++i) {
                seq_t term = bin * a_arr[i];
                term      = term * a_arr[nnow - i];
                sumC      += term;
                if (i < nnow) {
                    bin = bin * (seq_t)(nnow - i);
                    bin = bin / (seq_t)(i + 1);   /* next binomial */
                }
            }
            C_arr[nnow] = sumC;
        }

        /* ===== 2. Compute h(nnow) for nnow>0 ===== */
        if (nnow > 0) {
            seq_t    sumMag = 0;
            bool      sumSign = false;   /* positive sum initially */
            seq_t    bin = (seq_t)nnow;  /* binom(nnow,1) */
            for (idx_t i = 1; i <= nnow; ++i) {
                seq_t prod = bin * C_arr[i];
                prod      = prod * magH[nnow - i];
                bool termNeg = negH[nnow - i];
                add_signed_seq(&sumMag, &sumSign, prod, termNeg);
                if (i < nnow) {
                    bin = bin * (seq_t)(nnow - i);
                    bin = bin / (seq_t)(i + 1);
                }
            }
            /* h(nnow) = –sum */
            bool hNeg;
            seq_t hMag;
            if (sumMag == 0) {          /* zero is non‑negative */
                hNeg = false;
                hMag = 0;
            } else {
                hNeg = !sumSign;        /* opposite sign of sum */
                hMag = sumMag;
            }
            magH[nnow] = hMag;
            negH[nnow] = hNeg;
        }

        /* ===== 3. Compute a(nnow+1) ===== */
        seq_t sumA = 0;
        seq_t bin  = 1;   /* binom(nnow,0) */
        for (idx_t k = 0; k <= nnow; ++k) {
            seq_t term = bin * a_arr[k];
            term      = term * magH[nnow - k];
            sumA      += term;
            if (k < nnow) {
                bin = bin * (seq_t)(nnow - k);
                bin = bin / (seq_t)(k + 1);
            }
        }
        a_arr[nnow + 1] = sumA;
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(C_arr);
    free(magH);
    free(negH);
    return result;
}
