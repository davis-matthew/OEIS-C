/*
 * A320738  Number of partitions of n with seven sorts of part 1 which are
 *          introduced in ascending order.
 * https://oeis.org/A320738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The generating function is G(x) = (1 - 21*x + 170*x^2 - 669*x^3
 *            + 1314*x^4 - 1157*x^5 + 309*x^6) / ((1 - 2*x)(1 - 3*x)(1 -
 *            4*x)(1 - 5*x)(1 - 7*x) * Π_{k≥1}(1 - x^k)). Writing Q(x) = (1 -
 *            2*x)(1 - 3*x)(1 - 4*x)(1 - 5*x)(1 - 7*x) and P(x) = Π_{k≥1}(1 -
 *            x^k), we let B(n) be the coefficient of Q(x)P(...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 49,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static const int Q_coeff[6] = {1, -21, 169, -651, 1198, -840};
static const int L_coeff[7] = {1, -21, 170, -669, 1314, -1157, 309};

static seq_t *A = NULL;      /* cached a(n) values */
static size_t A_cap = 0;     /* allocated size of A */
static size_t A_len = 0;     /* number of values currently computed */

static int   *B = NULL;      /* B(n) coefficients */
static size_t B_cap = 0;     /* allocated size of B and P */

/* forward declarations */
static void compute_pP_and_B(idx_t N);

/* ensure that we have space for indices up to n */
static void ensure_size(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (A_cap < need) {
        A = (seq_t *)realloc(A, need * sizeof(seq_t));
        if (!A) abort();
        for (size_t i = A_cap; i < need; ++i) A[i] = 0;
        A_cap = need;
    }
    if (B_cap < need) {
        B = (int *)realloc(B, need * sizeof(int));
        if (!B) abort();
        /* pP will be recomputed in compute_pP_and_B */
        compute_pP_and_B(n);
        B_cap = need;
    }
}

/* compute coefficients of product (1-x^k) and Q(x)P(x) up to N */
static void compute_pP_and_B(idx_t N)
{
    size_t Nsize = (size_t)N;
    if (!B) {
        /* should never happen: ensure_size will allocate B first */
        return;
    }
    /* allocate temporary array for P(x) coefficients (pP) */
    int *pP = (int *)malloc((Nsize + 1) * sizeof(int));
    if (!pP) abort();
    for (size_t i = 0; i <= Nsize; ++i) pP[i] = 0;
    pP[0] = 1;                    /* coefficient of x^0 is 1 */

    /* Euler's pentagonal theorem */
    for (int m = 1; ; ++m) {
        int p1 = m * (3 * m - 1) / 2;
        if (p1 > (int)Nsize) break;
        int sign = (m % 2 == 0) ? 1 : -1;
        pP[p1] = sign;

        int p2 = m * (3 * m + 1) / 2;
        if (p2 <= (int)Nsize) pP[p2] = sign;
    }

    /* compute B[n] = Σ_{k=0}^{min(5,n)} Q_k * pP[n-k] */
    for (size_t n = 0; n <= Nsize; ++n) {
        int s = 0;
        int maxk = (n < 5 ? (int)n : 5);
        for (int k = 0; k <= maxk; ++k) {
            s += Q_coeff[k] * pP[n - k];
        }
        B[n] = s;
    }
    free(pP);
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_size(n);

    /* compute unknown a(n) values up to n */
    for (size_t idx = A_len; idx <= (size_t)n; ++idx) {
        seq_t sumPos = 0;
        seq_t sumNeg = 0;

        /* convolution sum for current idx */
        for (size_t i = 0; i < idx; ++i) {
            int coeff = B[idx - i];
            if (coeff > 0) {
                sumPos += mul_small(A[i], (unsigned)coeff);
            } else if (coeff < 0) {
                sumNeg += mul_small(A[i], (unsigned)(-coeff));
            }
        }

        seq_t result;
        if (idx <= 6) {
            /* result = sumNeg - sumPos + L[idx] */
            result = sumNeg;
            result -= sumPos;
            int L = L_coeff[(int)idx];
            if (L >= 0) {
                result += (seq_t)L;
            } else {
                result -= (seq_t)(-L);
            }
        } else {
            /* result = sumNeg - sumPos (since L=0) */
            result = sumNeg;
            result -= sumPos;
        }

        A[idx] = result;
    }
    A_len = (size_t)n + 1;    /* we have computed up to n */
    return A[(size_t)n];
}
