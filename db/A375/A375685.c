/*
 * A375685  Expansion of e.g.f. 1 / (1 + x^2/2 * log(1 - x))^2.
 * https://oeis.org/A375685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (k+1)! *
 *            |Stirling1(n-2*k,k)|/(2^k*(n-2*k)!). This implementation
 *            computes the sequence using its e.g.f. B(x) = 1 / (1 + (x^2/2) *
 *            log(1-x)) has coefficients b[n] (e.g.f). They satisfy the
 *            recurrence b[0] = 1, b[1] = 0, b[2] = 0 b[n] = Σ_{k=3..n} b[n-k]
 *            * P(n,k)...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_size(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : *cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr   = tmp;
        *cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *B = NULL;        /* coefficients of B(x)   */
    static seq_t *A = NULL;        /* target sequence a(n)   */
    static idx_t  current_max = 0; /* highest n which has been computed */
    static size_t B_cap = 0, A_cap = 0;

    /* Initialisation */
    if (B == NULL) {
        ensure_size(&B, &B_cap, 16);
        ensure_size(&A, &A_cap, 16);

        B[0] = (seq_t)1;  A[0] = (seq_t)1;
        if (B_cap > 1) { B[1] = (seq_t)0; A[1] = (seq_t)0; }
        if (B_cap > 2) { B[2] = (seq_t)0; A[2] = (seq_t)0; }
        current_max = (n <= 2 ? n : 2);
    }

    /* Grow to requested n */
    if (n > current_max) {
        size_t need = (size_t)n + 1;
        ensure_size(&B, &B_cap, need);
        ensure_size(&A, &A_cap, need);

        for (idx_t cur = current_max + 1; cur <= n; ++cur) {
            /* Compute B[cur] via recurrence */
            seq_t b_val;
            if (cur < 3) {
                b_val = (seq_t)0;          /* for cur==1,2 */
            } else {
                seq_t sum = 0;
                seq_t P   = 1;             /* P(cur,0) */
                for (idx_t k = 1; k <= cur; ++k) {
                    P *= (seq_t)(cur - k + 1);          /* P(cur,k) */
                    if (k >= 3) {
                        seq_t term = B[cur - k] * P;
                        term /= (seq_t)(2ULL * (k - 2));
                        sum += term;
                    }
                }
                b_val = sum;
            }
            if (cur == 0) b_val = (seq_t)1;   /* safety for n==0 */
            B[cur] = b_val;

            /* Compute A[cur] = Σ binom(cur,i) * B[i] * B[cur-i] */
            seq_t a_val = 0;
            seq_t binom = 1;                  /* binom(cur,0) */
            for (idx_t i = 0; i <= cur; ++i) {
                seq_t term = binom * B[i] * B[cur - i];
                a_val += term;
                if (i != cur) {
                    binom = binom * (cur - i);
                    binom = binom / (i + 1);   /* exact division */
                }
            }
            A[cur] = a_val;
        }
        current_max = n;
    }

    return A[n];
}
