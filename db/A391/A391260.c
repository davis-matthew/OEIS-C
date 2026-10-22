/*
 * A391260  Square array A(n,k): Index of the product of Stern polynomials
 *          B(n,x) and B(k,x) among the Stern polynomials, or 0 if the product
 *          is not a Stern polynomial (see A125184).
 * https://oeis.org/A391260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i*j if B_i(x)·B_j(x) equals the Stern polynomial
 *            B_{i*j}(x), otherwise a(n) = 0. Here i and j are the
 *            antidiagonal coordinates of index n
 * Range:     u8 n <= 480, u16 n <= 9337, u32 n <= 9337, u64 n <= 9337,
 *            u128 n <= 9337, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Maximum degree of any computed Stern polynomial.  For n up to 3·10⁴ the
   degree never exceeds 32.  Increase if a larger bound is needed.    */
#define MAX_DEG 32

/* Max polynomial index needed for n up to 10⁴ (see comments).      */
#define MAX_POLY_IDX 30000

/* A polynomial is represented by the degree and an array of coefficients
   (coeff[0] is the constant term).  The coefficient array is sized
   MAX_DEG+1 to accommodate the largest possible degree.           */
typedef struct {
    int deg;
    seq_t coeff[MAX_DEG+1];
} poly_t;

/* Global storage of all polynomials up to MAX_POLY_IDX.            */
static poly_t *poly_table = NULL;

/* ------------------------------------------------------------------ */
/* Helper functions                                                      */
/* ------------------------------------------------------------------ */
static void init_polynomials(void)
{
    if (poly_table != NULL) return;              /* already initialized */

    poly_table = (poly_t *)calloc(MAX_POLY_IDX + 1, sizeof(poly_t));
    if (!poly_table) abort();                    /* cannot allocate */

    /* 0: B_0 = 0                                           */
    poly_table[0].deg = 0;
    poly_table[0].coeff[0] = 0;

    /* 1: B_1 = 1                                           */
    poly_table[1].deg = 0;
    poly_table[1].coeff[0] = 1;

    for (idx_t n = 2; n <= (idx_t)MAX_POLY_IDX; ++n) {
        if ((n & 1ULL) == 0) {                  /* even n:  B_n = x * B_{n/2} */
            idx_t half = n >> 1;
            poly_t *p = &poly_table[half];
            poly_t *q = &poly_table[n];
            q->deg = p->deg + 1;
            q->coeff[0] = 0;
            for (int k = 0; k <= p->deg; ++k) {
                q->coeff[k + 1] = p->coeff[k];
            }
            for (int k = q->deg + 1; k <= MAX_DEG; ++k) {
                q->coeff[k] = 0;
            }
        } else {                                /* odd n:   B_n = B_{(n-1)/2} + B_{(n+1)/2} */
            idx_t a = n >> 1;
            idx_t b = a + 1;
            poly_t *pa = &poly_table[a];
            poly_t *pb = &poly_table[b];
            poly_t *q   = &poly_table[n];
            int d = (pa->deg > pb->deg) ? pa->deg : pb->deg;
            q->deg = d;
            for (int k = 0; k <= d; ++k) {
                seq_t ca = (k <= pa->deg) ? pa->coeff[k] : (seq_t)0;
                seq_t cb = (k <= pb->deg) ? pb->coeff[k] : (seq_t)0;
                q->coeff[k] = ca + cb;
            }
            for (int k = d + 1; k <= MAX_DEG; ++k) {
                q->coeff[k] = 0;
            }
        }
    }
}

/* Compare two polynomials for equality. */
static bool polys_equal(const poly_t *p, const poly_t *q)
{
    if (p->deg != q->deg) return false;
    for (int k = 0; k <= p->deg; ++k)
        if (p->coeff[k] != q->coeff[k]) return false;
    return true;
}

/* Multiply two polynomials; result stored in *res.  All operations are
   performed using seq_t arithmetic. */
static void poly_mult(const poly_t *a, const poly_t *b, poly_t *res)
{
    int da = a->deg, db = b->deg;
    int dres = da + db;
    res->deg = dres;
    for (int k = 0; k <= dres; ++k) res->coeff[k] = 0;
    for (int i = 0; i <= da; ++i) {
        for (int j = 0; j <= db; ++j) {
            res->coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
}

/* Compute the antidiagonal coordinates (i, j) for the given index n.
   The function assumes n >= 1.  The coordinates satisfy i + j = s,
   where s is the smallest integer with (s-1)s/2 >= n. */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Compute approximate s using double arithmetic; n <= 10⁴ so no loss. */
    double ds = sqrt(1.0 + 8.0 * (double)n);
    idx_t s = (idx_t)((1.0 + ds) * 0.5);
    while ((s - 1ULL) * s / 2ULL < n) ++s;
    idx_t total_prev = (s - 1ULL) * (s - 2ULL) / 2ULL;
    idx_t offset = n - total_prev;
    *i = offset;
    *j = s - offset;
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                     */
/* ------------------------------------------------------------------ */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1: ignore n==0 */

    /* Ensure that all required Stern polynomials are available. */
    init_polynomials();

    /* Convert linear index to antidiagonal coordinates. */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Product index would be i*j if the product is a Stern polynomial. */
    idx_t prod_idx = i * j;
    if (prod_idx > (idx_t)MAX_POLY_IDX) return (seq_t)0;

    /* Multiply polynomials B_i and B_j. */
    static poly_t prod;                     /* reuse allocation */
    poly_mult(&poly_table[i], &poly_table[j], &prod);

    /* Compare with B_{i*j}. */
    if (polys_equal(&poly_table[prod_idx], &prod))
        return (seq_t)prod_idx;
    else
        return (seq_t)0;
}
