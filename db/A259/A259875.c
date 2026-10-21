/*
 * A259875  Irregular triangle read by rows: coefficients (highest degree
 *          first) of polynomials defined by p_0(x)=0, p_1(x)=p_2(x)=1,
 *          p_3(x)=x+1; p_n(x)=x*p_{n-2}(x)-p_{n-4}(x).
 * https://oeis.org/A259875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, look
 * Formula:   a(n) = coefficient of the monic term in the irregular triangle
 *            read by rows. The triangle is defined by the polynomials
 *            p_0(x)=0, p_1(x)=p_2(x)=1, p_3(x)=x+1,
 *            p_n(x)=x*p_{n-2}(x)-p_{n-4}(x) (n≥4). a(n) is the n‑th
 *            coefficient in that triangle when read row by row (highest
 *            degree first)
 * Range:     i8 n <= 216, i16 n <= 714, i32 n <= 2476, i64 n <= 9362,
 *            i128 n <= 9633, big any n
 * Helpers:   poly_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/poly_len.h"

/* cached coefficient arrays */
static seq_t **poly_array = NULL;         /* array of pointers to coefficients */
static size_t array_cap  = 0;             /* allocated size of poly_array */
static idx_t   built_max = (idx_t)-1;     /* highest n for which poly_array[n] is built */

/* make poly_array large enough for indices up to k */
static void ensure_capacity(idx_t k)
{
    if (array_cap < (size_t)(k + 1)) {
        size_t new_cap = (array_cap == 0) ? 4 : array_cap * 2;
        if (new_cap <= (size_t)k)
            new_cap = (size_t)k + 1;
        seq_t **tmp = (seq_t**)realloc(poly_array, new_cap * sizeof(seq_t*));
        if (!tmp) return;                     /* allocation failed – give up */
        for (size_t i = array_cap; i < new_cap; ++i)
            tmp[i] = NULL;
        poly_array = tmp;
        array_cap = new_cap;
    }
}

/* build all polynomials from built_max+1 up to k */
static void build_polys_up_to(idx_t k)
{
    if (built_max != (idx_t)-1 && built_max >= k) return;

    for (idx_t i = built_max + 1; i <= k; ++i) {
        if (i == 0) {
            poly_array[0] = (seq_t*)malloc(sizeof(seq_t));
            poly_array[0][0] = (seq_t)0;
        } else if (i == 1 || i == 2) {
            poly_array[i] = (seq_t*)malloc(sizeof(seq_t));
            poly_array[i][0] = (seq_t)1;
        } else if (i == 3) {
            poly_array[3] = (seq_t*)malloc(2 * sizeof(seq_t));
            poly_array[3][0] = (seq_t)1;   /* coefficient of x   */
            poly_array[3][1] = (seq_t)1;   /* constant term      */
        } else {
            seq_t *A = poly_array[i - 2];
            seq_t *B = poly_array[i - 4];
            size_t lenA  = poly_len(i - 2);
            size_t lenB  = poly_len(i - 4);
            size_t newlen = poly_len(i);
            seq_t *C = (seq_t*)calloc(newlen, sizeof(seq_t)); /* zeroed */
            /* x * p_{i-2} */
            for (size_t t = 0; t < lenA; ++t)
                C[t] = A[t];
            /* - p_{i-4} */
            size_t offset = newlen - lenB;
            for (size_t t = 0; t < lenB; ++t)
                C[offset + t] -= B[t];
            poly_array[i] = C;
        }
    }
    built_max = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* p_0(x)=0 gives first coefficient 0 */

    /* locate the row k and column j of the n‑th coefficient */
    idx_t sum = 0;
    idx_t k = 0;
    while (true) {
        size_t len = poly_len(k);
        if (sum + len > n) break;
        sum += len;
        ++k;
    }
    size_t j = (size_t)(n - sum);   /* 0‑based column index */

    ensure_capacity(k);
    build_polys_up_to(k);

    return poly_array[k][j];
}
