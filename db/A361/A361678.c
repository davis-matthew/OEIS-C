/*
 * A361678  Constant term in the expansion of (1 + w + x + y + z + 1/(x*y*z) +
 *          1/(w*y*z) + 1/(w*x*z) + 1/(w*x*y))^n.
 * https://oeis.org/A361678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (4k)!/(k!^4) * binomial(4k,k) *
 *            binomial(n,4k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Ensure that a dynamic array of seq_t has at least capacity n.  */
static void ensure_seq_capacity(seq_t **arr, size_t *cap, size_t n) {
    if (*cap < n) {
        seq_t *p = (seq_t *)realloc(*arr, n * sizeof(seq_t));
        if (!p && n > 0) {
            exit(1);
        }
        *arr = p;
        *cap = n;
    }
}

seq_t a(idx_t n) {
    /* Static tables for coefficients that depend only on k.            */
    static seq_t *C = NULL;          /* C[k] = (4k)!/(k!^4)                     */
    static seq_t *B = NULL;          /* B[k] = binomial(4k,k)                  */
    static size_t cap_C = 0, cap_B = 0;
    static size_t max_k = 0;         /* largest k for which C/B have been filled */

    /* Compute the maximum k needed for this n. */
    idx_t kmax_idx = n / 4;          /* floor(n/4) */
    size_t kmax = (size_t)kmax_idx;

    /* Initialise coefficient tables on first use. */
    if (max_k == 0 && cap_C == 0) {
        ensure_seq_capacity(&C, &cap_C, 1);
        ensure_seq_capacity(&B, &cap_B, 1);
        C[0] = (seq_t)1;
        B[0] = (seq_t)1;
        max_k = 0;
    }

    /* Grow tables if a larger k is required. */
    if (kmax > max_k) {
        ensure_seq_capacity(&C, &cap_C, kmax + 1);
        ensure_seq_capacity(&B, &cap_B, kmax + 1);

        for (size_t kk = max_k + 1; kk <= kmax; ++kk) {
            /* C[kk] = C[kk-1] * (4kk)*(4kk-1)*(4kk-2)*(4kk-3) / (kk^4) */
            seq_t num   = ((seq_t)(4 * kk)) * ((seq_t)(4 * kk - 1))
                         * ((seq_t)(4 * kk - 2)) * ((seq_t)(4 * kk - 3));
            seq_t denom = ((seq_t)kk) * ((seq_t)kk) * ((seq_t)kk) * ((seq_t)kk);
            C[kk] = C[kk - 1] * num / denom;

            /* B[kk] = B[kk-1] * num / (kk * (3kk-2)*(3kk-1)*(3kk)) */
            seq_t denom2 = ((seq_t)kk) * ((seq_t)(3 * kk - 2))
                           * ((seq_t)(3 * kk - 1)) * ((seq_t)(3 * kk));
            B[kk] = B[kk - 1] * num / denom2;
        }
        max_k = kmax;
    }

    /* Build binomial(n, m) for m = 0..4*kmax (needed only for m = 4*k). */
    static seq_t *binom_arr = NULL;
    static size_t binom_cap = 0;
    size_t binom_len = 4 * kmax + 1;

    if (binom_cap < binom_len) {
        seq_t *p = (seq_t *)realloc(binom_arr, binom_len * sizeof(seq_t));
        if (!p && binom_len > 0) {
            exit(1);
        }
        binom_arr = p;
        binom_cap = binom_len;
    }

    binom_arr[0] = (seq_t)1;
    for (size_t m = 1; m < binom_len; ++m) {
        seq_t factor = (seq_t)(n - (idx_t)(m - 1));
        binom_arr[m] = binom_arr[m - 1] * factor / (seq_t)m;
    }

    /* Sum the terms. */
    seq_t result = (seq_t)0;
    for (size_t kk = 0; kk <= kmax; ++kk) {
        seq_t term = C[kk] * B[kk];
        term = term * binom_arr[4 * kk];
        result += term;
    }

    return result;
}
