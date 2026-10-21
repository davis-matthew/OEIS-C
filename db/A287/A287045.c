/*
 * A287045  a(n) is the number of size n affine closed terms of variable size
 *          0.
 * https://oeis.org/A287045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A287045: a(n) = number of size‑n affine closed terms of variable size 0
   Recurrence (from OEIS):
     a(0)=0, a(1)=1, a(2)=2, a(3)=8
     a(n) = (3*a(n‑1) + (6*n‑10)*a(n‑2) – a(n‑3) + 2*b(n‑1) – b(n‑2) – b(n‑3))/2
     where b(n) = Σ_{k=1}^{n‑1} a(k)*a(n‑k)  (b(0) = 0)               */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *A = NULL;        /* cache of a(n) values          */
static idx_t  A_cap = 0;        /* allocated capacity (>0)       */
static idx_t  A_len = 0;        /* number of values computed    */

static seq_t *B = NULL;        /* cache of b(n) values          */
static idx_t  B_cap = 0;        /* allocated capacity (>0)       */
static idx_t  B_len = 0;        /* number of values computed    */

/* Allocate a new array of size new_cap and copy existing contents.  */
static void ensure_capacity(idx_t n, seq_t **arr, idx_t *cap)
{
    if (n + 1 > *cap) {
        idx_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap <= n) new_cap <<= 1;

        seq_t *new_arr = NULL;
#if defined(__cplusplus)
        new_arr = new seq_t[new_cap];
        if (*arr) {
            for (idx_t i = 0; i < *cap; ++i) new_arr[i] = (*arr)[i];
            delete[] *arr;
        }
#else
        new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (*arr) {
            memcpy(new_arr, *arr, (*cap) * sizeof(seq_t));
            free(*arr);
        }
#endif
        *arr  = new_arr;
        *cap  = new_cap;
    }
}

/* Initialize the caches with the first four terms.  */
static void init_arrays(void)
{
    if (A_cap == 0) {
        ensure_capacity(3, &A, &A_cap);
        A[0] = (seq_t)0;
        A[1] = (seq_t)1;
        A[2] = (seq_t)2;
        A[3] = (seq_t)8;
        A_len = 4;
    }
    if (B_cap == 0) {
        ensure_capacity(3, &B, &B_cap);
        B[0] = (seq_t)0;                /* unused */
        B[1] = (seq_t)0;                /* empty sum */
        B[2] = (seq_t)1;                /* a1*a1        */
        B[3] = (seq_t)4;                /* a1*a2 + a2*a1*/
        B_len = 4;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_arrays();

    if (n < A_len) return A[n];

    ensure_capacity(n, &A, &A_cap);
    ensure_capacity(n, &B, &B_cap);

    for (idx_t k = A_len; k <= n; ++k) {
        /* compute b(k) = Σ_{i=1}^{k-1} a(i)*a(k-i) */
        if (k >= 1) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i < k; ++i)
                sum += A[i] * A[k - i];
            B[k] = sum;
        }

        if (k == 1) {
            A[k] = (seq_t)1;
        } else if (k == 2) {
            A[k] = (seq_t)2;
        } else if (k == 3) {
            A[k] = (seq_t)8;
        } else {
            seq_t term = 3 * A[k - 1];
            term += (seq_t)((6 * k - 10)) * A[k - 2];
            term -= A[k - 3];
            term += 2 * B[k - 1];
            term -= B[k - 2];
            term -= B[k - 3];
            term /= (seq_t)2;          /* division is exact */
            A[k] = term;
        }
    }

    A_len = n + 1;
    B_len = n + 1;
    return A[n];
}
