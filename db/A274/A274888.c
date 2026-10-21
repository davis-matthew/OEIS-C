/*
 * A274888  Triangle read by rows: the q-analog of the swinging factorial
 *          which is defined as q-multinomial([floor(n/2), n mod 2,
 *          floor(n/2)]).
 * https://oeis.org/A274888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of q^k in the q-swing_factorial polynomial
 *            read by rows. The sequence is the concatenation of the
 *            coefficient vectors of the polynomials q-swing_factorial(n) for
 *            n = 0, 1, 2, ... See OEIS A274888
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *coeffs = NULL;      /* global list of coefficients read by rows */
static size_t coeffs_len = 0;     /* number of coefficients currently stored */
static size_t coeffs_cap = 0;     /* capacity of coeffs array */

static seq_t *polyPrev = NULL;    /* polynomial of the previous row (n-1)   */
static size_t polyPrev_len = 0;    /* number of coefficients in polyPrev   */
static size_t polyPrev_cap = 0;    /* capacity of polyPrev array           */

static idx_t curRow = 0;          /* last computed row index              */
static int initialized = 0;       /* flag indicating that global state   */

/* Grow a sequence array to at least 'needed' elements */
static void ensure_cap(seq_t **arr, size_t *len, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap *= 2;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise global state on first call */
    if (!initialized) {
        coeffs_cap = 16;
        coeffs = (seq_t *)malloc(coeffs_cap * sizeof(seq_t));
        coeffs_len = 0;

        /* row 0 polynomial: 1 */
        polyPrev_cap = 8;
        polyPrev = (seq_t *)malloc(polyPrev_cap * sizeof(seq_t));
        polyPrev[0] = 1;
        polyPrev_len = 1;
        curRow = 0;

        /* store its coefficients */
        ensure_cap(&coeffs, &coeffs_len, &coeffs_cap, coeffs_len + 1);
        coeffs[coeffs_len++] = 1;

        initialized = 1;
    }

    /* generate rows until we have at least n+1 coefficients */
    while (coeffs_len <= (size_t)n) {
        curRow++;   /* next row to compute */

        /* --- compute polynomial for row curRow --- */
        seq_t *polyCur = NULL;
        size_t polyCur_len = 0;

        if (curRow % 2 == 1) {                /* odd n: multiply by [n] */
            idx_t m = curRow;                 /* [n] has length m of ones */
            size_t newLen = polyPrev_len + (size_t)m - 1; /* convolution length */
            polyCur = (seq_t *)malloc(newLen * sizeof(seq_t));
            for (size_t i = 0; i < newLen; ++i) polyCur[i] = 0;

            for (size_t i = 0; i < polyPrev_len; ++i) {
                seq_t val = polyPrev[i];
                for (size_t j = 0; j < (size_t)m; ++j)
                    polyCur[i + j] += val;
            }
            polyCur_len = newLen;
        } else {                              /* even n: multiply by (1+q^m) / [m] */
            idx_t m = curRow / 2;              /* m >= 1 */
            size_t newLen = polyPrev_len + (size_t)m;   /* after shift and add */

            seq_t *temp = (seq_t *)malloc(newLen * sizeof(seq_t));
            for (size_t i = 0; i < newLen; ++i) temp[i] = 0;

            for (size_t i = 0; i < polyPrev_len; ++i) {
                seq_t val = polyPrev[i];
                temp[i] += val;
                temp[i + (size_t)m] += val;
            }

            if (m == 1) {                     /* divisor [1] = 1, no division needed */
                polyCur = temp;
                polyCur_len = newLen;
            } else {
                size_t lenD = (size_t)m;       /* length of divisor, all ones */
                size_t lenQ = newLen - (lenD - 1);
                polyCur = (seq_t *)malloc(lenQ * sizeof(seq_t));
                for (size_t i = 0; i < lenQ; ++i) polyCur[i] = 0;

                /* long division: temp = polyCur * divisor */
                for (long long k = (long long)newLen - 1;
                     k >= (long long)lenD - 1; --k) {
                    seq_t coeff = temp[(size_t)k];
                    size_t qidx = (size_t)k - (lenD - 1);
                    polyCur[qidx] = coeff;
                    for (size_t j = 0; j < lenD; ++j) {
                        temp[qidx + j] -= coeff;
                    }
                }
                free(temp);
                polyCur_len = lenQ;
            }
        }

        /* append new polynomial coefficients to global list */
        ensure_cap(&coeffs, &coeffs_len, &coeffs_cap, coeffs_len + polyCur_len);
        for (size_t i = 0; i < polyCur_len; ++i)
            coeffs[coeffs_len++] = polyCur[i];

        /* prepare for next iteration: previous polynomial becomes current */
        free(polyPrev);
        polyPrev = polyCur;
        polyPrev_len = polyCur_len;
    }

    return coeffs[(size_t)n];
}
