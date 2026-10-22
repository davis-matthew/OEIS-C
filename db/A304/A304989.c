/*
 * A304989  G.f.: Sum_{k>=0} A000041(k)^2 * x^k / Sum_{k>=0} A000009(k)^2 *
 *          x^k.
 * https://oeis.org/A304989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of the generating function (Σ
 *            A000041(k)^2 · x^k) / (Σ A000009(k)^2 · x^k). Equivalently, let
 *            b_n = A000041(n)^2 and c_n = A000009(n)^2. Then b_n = Σ_{i=0}^n
 *            a_i · c_{n-i}
 * Range:     i8 n <= 7, i16 n <= 19, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *P = NULL;          /* partition numbers A000041 */
static seq_t *Q = NULL;          /* distinct‑partitions A000009 */
static seq_t *B = NULL;          /* P[n]^2 */
static seq_t *C = NULL;          /* Q[n]^2 */
static seq_t *A = NULL;          /* required sequence */
static idx_t   Max = 0;          /* highest index computed */

/* enlarge tables to contain values up to n (inclusive) */
static void ensure_size(idx_t n)
{
    if (n + 1 <= Max) return;          /* already big enough */

    idx_t sz = n + 1;
    P = (seq_t *)realloc(P, sz * sizeof(seq_t));
    Q = (seq_t *)realloc(Q, sz * sizeof(seq_t));
    B = (seq_t *)realloc(B, sz * sizeof(seq_t));
    C = (seq_t *)realloc(C, sz * sizeof(seq_t));
    A = (seq_t *)realloc(A, sz * sizeof(seq_t));

    /* reset all entries */
    for (idx_t i = 0; i < sz; ++i) {
        P[i] = 0;
        Q[i] = 0;
        B[i] = 0;
        C[i] = 0;
        A[i] = 0;
    }

    /* base cases */
    P[0] = 1;
    Q[0] = 1;
    B[0] = 1;               /* 1^2 */
    C[0] = 1;               /* 1^2 */
    A[0] = 1;               /* b_0 / c_0 = 1 */

    /* ---------- unrestricted partitions P[n] ----------
       dynamic programming: for each part size add to all larger sums  */
    for (idx_t part = 1; part < sz; ++part) {
        for (idx_t sum = part; sum < sz; ++sum) {
            P[sum] += P[sum - part];
        }
    }

    /* ---------- distinct‑partitions Q[n] ----------
       subset sum DP: each part can be used at most once  */
    for (idx_t part = 1; part < sz; ++part) {
        for (idx_t sum = sz - 1; sum >= part; --sum) {
            Q[sum] += Q[sum - part];
        }
    }

    /* square values to obtain B[n] and C[n] */
    for (idx_t i = 0; i < sz; ++i) {
        B[i] = P[i] * P[i];
        C[i] = Q[i] * Q[i];
    }

    /* ---------- convolution to obtain a[n] ----------
       b_n = Σ_{i=0}^n a_i · c_{n-i}  =>  a_n = b_n - Σ_{i=0}^{n-1} a_i · c_{n-i}
       (c_0 = 1, so no division needed)  */
    for (idx_t nidx = 1; nidx < sz; ++nidx) {
        seq_t sum = 0;
        for (idx_t i = 0; i < nidx; ++i) {
            sum += A[i] * C[nidx - i];
        }
        A[nidx] = B[nidx] - sum;
    }

    Max = sz;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return A[n];
}
