/*
 * A399549  Irregular triangle T(n, k) read by rows. T(n, k) gives the
 *          coefficient [t^k] P_{n}(t) from the Somos-4 sequence of
 *          polynomials P_{0}(t) = P_{1}(t) = P_{2}(t) = P_{3}(t)=1; for n >=
 *          4, P_{n}(t) = (P_{n-1}(t) * P_{n-3}(t) + t*P_{n-2}(t)^2) /
 *          P_{n-4}(t).
 * https://oeis.org/A399549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: coefficients T(n,k) of the Somos‑4 polynomials P_n(t)
   defined by
        P_0 = P_1 = P_2 = P_3 = 1
        P_n = (P_{n-1} * P_{n-3} + t · P_{n-2}²) / P_{n-4}  (n ≥ 4)
   a(n) is the n‑th element of the flattened triangle
   (row 0 → 1, row 1 → 1, …). */

#include <stdlib.h>
#include <stdbool.h>

/* static mutable state for memoisation */
static bool initialized = false;
static size_t polys_cap = 0;          /* capacity of arrays */
static size_t polys_n = 0;            /* number of computed polynomials */
static seq_t **polys_coeffs = NULL;   /* coeff arrays */
static size_t *polys_len = NULL;      /* length of each poly */
static size_t *row_len = NULL;        /* length of each row */
static idx_t *cum_row_len = NULL;     /* cumulative row lengths */

/* ensure arrays have at least <need> capacity */
static void grow_arrays(size_t need) {
    while (polys_cap < need) {
        size_t new_cap = polys_cap ? polys_cap * 2 : 16;
        polys_coeffs = (seq_t**)realloc(polys_coeffs, new_cap * sizeof(seq_t*));
        polys_len    = (size_t*)realloc(polys_len,    new_cap * sizeof(size_t));
        row_len      = (size_t*)realloc(row_len,      new_cap * sizeof(size_t));
        cum_row_len  = (idx_t*)realloc(cum_row_len,  new_cap * sizeof(idx_t));
        polys_cap = new_cap;
    }
}

/* initialise base polynomials and first rows */
static void init_once(void) {
    grow_arrays(16);

    /* base polynomials P_0 … P_3 = 1 */
    for (idx_t i = 0; i < 4; ++i) {
        polys_coeffs[i] = (seq_t*)malloc(sizeof(seq_t));
        polys_coeffs[i][0] = (seq_t)1;
        polys_len[i] = 1;
        row_len[i] = 1;
    }

    /* cumulative row lengths */
    cum_row_len[0] = 1;
    for (idx_t i = 1; i < 4; ++i)
        cum_row_len[i] = cum_row_len[i-1] + (idx_t)row_len[i];

    polys_n = 4;
    initialized = true;
}

/* ensure rows up to a target cumulative length > n */
static void ensure_rows(idx_t n) {
    if (!initialized) init_once();

    while (cum_row_len[polys_n - 1] <= n) {
        idx_t m = polys_n;                     /* new polynomial index */
        /* compute length of this row */
        size_t len;
        static const size_t init_len[10] = {1,1,1,1,2,2,4,5,7,9};
        if (m < 10) {
            len = init_len[(size_t)m];
        } else {
            len = 2ULL * row_len[m-1] - row_len[m-2]
                + row_len[m-7] - 2ULL * row_len[m-8] + row_len[m-9];
        }

        grow_arrays(polys_n + 1);

        /* store row information */
        row_len[m] = len;
        cum_row_len[m] = cum_row_len[m-1] + (idx_t)len;

        /* pointers to previous polynomials */
        seq_t *c1 = polys_coeffs[m-1];        size_t l1 = polys_len[m-1];
        seq_t *c3 = polys_coeffs[m-3];        size_t l3 = polys_len[m-3];
        seq_t *c2 = polys_coeffs[m-2];        size_t l2 = polys_len[m-2];
        seq_t *d  = polys_coeffs[m-4];        size_t ld = polys_len[m-4];

        /* A = c1 * c3 */
        size_t lA = l1 + l3 - 1;
        seq_t *A = (seq_t*)calloc(lA, sizeof(seq_t));
        for (size_t i = 0; i < l1; ++i)
            for (size_t j = 0; j < l3; ++j)
                A[i+j] += c1[i] * c3[j];

        /* B = c2 * c2 */
        size_t lB = l2 + l2 - 1;
        seq_t *B = (seq_t*)calloc(lB, sizeof(seq_t));
        for (size_t i = 0; i < l2; ++i)
            for (size_t j = 0; j < l2; ++j)
                B[i+j] += c2[i] * c2[j];

        /* Bshift = t * B  (shift right by one) */
        size_t lBshift = lB + 1;
        seq_t *Bshift = (seq_t*)calloc(lBshift, sizeof(seq_t));
        for (size_t i = 0; i < lB; ++i)
            Bshift[i+1] = B[i];                /* first element stays 0 */

        /* C = A + Bshift */
        size_t lC = lA > lBshift ? lA : lBshift;
        seq_t *C = (seq_t*)calloc(lC, sizeof(seq_t));
        for (size_t i = 0; i < lA; ++i) C[i] += A[i];
        for (size_t i = 0; i < lBshift; ++i) C[i] += Bshift[i];

        /* Division: N = C, divisor = d (constant term of d is 1) */
        size_t degN = lC - 1;
        size_t degD = ld - 1;
        size_t degQ = degN - degD;
        seq_t *Q = (seq_t*)calloc(degQ + 1, sizeof(seq_t));

        for (size_t k = 0; k <= degQ; ++k) {
            seq_t val = C[k];                     /* N[k] */
            size_t max_i = k < degD ? k : degD;    /* min(k,degD) */
            for (size_t i = 1; i <= max_i; ++i) {  /* d[i] corresponds to degree i */
                val -= Q[k-i] * d[i];
            }
            Q[k] = val;                           /* d[0] == 1 */
        }

        /* free temporaries */
        free(A);
        free(B);
        free(Bshift);
        free(C);

        /* store new polynomial */
        polys_coeffs[m] = Q;
        polys_len[m] = degQ + 1;

        ++polys_n;
    }
}

/* public function required by the grader */
seq_t a(idx_t n) {
    ensure_rows(n);

    /* find row containing the n‑th element */
    idx_t row = 0;
    while (cum_row_len[row] <= n) ++row;

    idx_t prev = (row == 0) ? 0 : cum_row_len[row - 1];
    size_t col = (size_t)(n - prev);          /* index within the row */

    return polys_coeffs[row][col];
}
