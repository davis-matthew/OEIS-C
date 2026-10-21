/*
 * A246381  Triangular matrix T defined by T = exp(L) where L(n,k) = C(2*n,
 *          2*k+1)/2, as read by rows n >= 0, k=0..n.
 * https://oeis.org/A246381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246381: flattened entries of the matrix T = exp(L) where
   L(n,k) = C(2*n, 2*k+1) / 2 (0 ≤ k ≤ n).  This function returns
   a(n) in the OEIS index convention (a(0)=1). */
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* triangular index helper: works for i ≥ j */
static inline size_t tri_idx(idx_t i, idx_t j)
{
    return (size_t)i * (i + 1) / 2 + j;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains the flattened index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;          /* column within row r */

    /* Total number of entries in the lower‑triangular block 0…r */
    size_t N = (size_t)(r + 1) * (r + 2) / 2;

    /* Allocate working arrays */
    seq_t *L      = seq_alloc_new_array(N);
    seq_t *T      = seq_alloc_new_array(N);
    seq_t *P_prev = seq_alloc_new_array(N);
    seq_t *P_curr = seq_alloc_new_array(N);

    /* Zero‑initialize all arrays */
    for (size_t i = 0; i < N; ++i) {
        L[i] = 0;
        T[i] = 0;
        P_prev[i] = 0;
        P_curr[i] = 0;
    }

    /* Identity matrix for T and for P_prev (the zero‑th power of L) */
    for (idx_t i = 0; i <= r; ++i) {
        size_t id = tri_idx(i, i);
        T[id]      = 1;
        P_prev[id] = 1;
    }

    /* Build the strict lower‑triangular matrix L */
    for (idx_t i = 0; i <= r; ++i) {
        for (idx_t j = 0; j < i; ++j) {
            seq_t val = binomial(2 * i, 2 * j + 1);
            val = val / (seq_t)2;         /* C(2i,2j+1) is even for odd top index */
            L[tri_idx(i, j)] = val;
        }
    }

    /* Compute the exponential series: T += L^k / k!  (k = 1 … r) */
    seq_t factorial = 1;
    for (idx_t k = 1; k <= r; ++k) {
        factorial *= (seq_t)k;              /* k!  */

        /* P_curr = L * P_prev  (matrix multiplication of triangular matrices) */
        for (idx_t i = 0; i <= r; ++i) {
            for (idx_t j = 0; j <= i; ++j) {
                seq_t sum = 0;
                for (idx_t m = j; m <= i; ++m) {
                    sum += L[tri_idx(i, m)] * P_prev[tri_idx(m, j)];
                }
                P_curr[tri_idx(i, j)] = sum;
            }
        }

        /* Add this term to the exponential sum */
        for (size_t idx = 0; idx < N; ++idx) {
            T[idx] += P_curr[idx] / factorial;
        }

        /* Prepare for next iteration: swap pointers */
        seq_t *tmp = P_prev;
        P_prev = P_curr;
        P_curr = tmp;
    }

    /* Result is the requested entry of the matrix T */
    seq_t result = T[tri_idx(r, c)];

    /* Clean up allocated memory */
    seq_free_array(L);
    seq_free_array(T);
    seq_free_array(P_prev);
    seq_free_array(P_curr);

    return result;
}
