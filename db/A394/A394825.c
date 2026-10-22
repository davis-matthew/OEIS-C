/*
 * A394825  Array read by ascending antidiagonals: A(n, k) = Sum_{j=0..k}
 *          binomial(k, j) * j^(n + j) * (1 - j)^(k - j).
 * https://oeis.org/A394825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Array read by ascending antidiagonals A(n,k) = Sum_{j=0..k}
 *            binomial(k,j) * j^(n+j) * (1-j)^(k-j) a(m) is the m‑th element
 *            in the read‑by‑diagonals sequence. Recurrence used for
 *            computation: A(n,0) = 0^n (0^0 = 1, else 0) A(0,k) =
 *            k*A(0,k-1)+1 A(n,k) = k*(A(n,k-1)+A(n-1,k)-A(n-1,k-1)) (n,k>0)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 85, u64 n <= 202,
 *            u128 n <= 545, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;          /* flattened triangle A[i][k] */
static size_t A_cap = 0;         /* current dimension (size = A_cap^2) */

/* Allocate or reallocate a square matrix of size new_cap */
static void rebuild_triangle(size_t new_cap)
{
    if (new_cap <= A_cap) return;             /* nothing to do */

    /* Allocate new storage */
    size_t new_size = new_cap * new_cap;
    seq_t *newA = (seq_t*)realloc(A, new_size * sizeof(seq_t));
    if (!newA) { /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    /* Fill new rows (if any) */
    for (size_t i = 0; i < new_cap; ++i) {
        for (size_t k = 0; k < new_cap; ++k) {
            size_t idx = i * new_cap + k;
            if (i == 0 && k == 0) {
                newA[idx] = 1;
            } else if (i == 0) {
                newA[idx] = (seq_t)k * newA[0 * new_cap + (k - 1)] + 1;
            } else if (k == 0) {
                newA[idx] = (i == 0) ? 1 : (seq_t)0;
            } else {
                seq_t v1 = newA[i * new_cap + (k - 1)];       /* A(i,k-1) */
                seq_t v2 = newA[(i - 1) * new_cap + k];       /* A(i-1,k) */
                seq_t v3 = newA[(i - 1) * new_cap + (k - 1)]; /* A(i-1,k-1) */
                newA[idx] = (seq_t)k * (v1 + v2 - v3);
            }
        }
    }

    A = newA;
    A_cap = new_cap;
}

/* Ensure that at least t+1 rows/cols exist (needed for diagonal t). */
static void ensure_triangle(size_t t)
{
    if (A_cap <= t) rebuild_triangle(t + 1);
}

seq_t a(idx_t n)
{
    /* Find diagonal t such that cumulative(t-1) < n ≤ cumulative(t) */
    size_t t = 0;
    while (((size_t)(t + 1) * (t + 2)) / 2 <= (size_t)n) {
        ++t;
    }

    /* Position within diagonal */
    size_t prev_cum = ((size_t)t * (t + 1)) / 2;
    size_t r = (size_t)n - prev_cum;

    /* Compute A(i,k) with i = t-r, k = r */
    size_t i = t - r;
    size_t k = r;

    /* Resize storage if needed */
    ensure_triangle(t);

    return A[i * (t + 1) + k];
}
