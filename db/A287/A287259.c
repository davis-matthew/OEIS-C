/*
 * A287259  Number of set partitions of [n] such that all absolute differences
 *          between least elements of consecutive blocks are <= nine.
 * https://oeis.org/A287259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A287216(n,9) – number of partitions of [n] with all
 *            absolute differences between consecutive block minima ≤ 9
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic programming approach.
   For each possible number of blocks k, we compute the weight
   of all sequences of block minima, and then accumulate the
   contributions to a(n).  The algorithm runs in O(n²) time
   and O(n) additional memory, with n being the argument of
   the function.  It is valid C11 and C++17.  No dynamic‑
   programming tables need to be stored in a separate file,
   all allocations are done inside the function. */

#define SEQ_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>

extern seq_t pow_seq(seq_t base, idx_t exp);  /* generic power if needed */

seq_t a(idx_t n) {
    /* Handle the offset n = 0 explicitly. */
    if (n == 0) return (seq_t)1;

    /* Convert n to a size_t for array indexing. */
    size_t N = (size_t)n;

    /* Allocate array for the final sequence values. */
    seq_t *ans = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!ans) abort();          /* allocation failure */
    ans[0] = (seq_t)1;          /* a(0) = 1 */

    /* k = 1 contribution: exactly one block for every n. */
    for (size_t i = 1; i <= N; ++i) ans[i] = (seq_t)1;

    /* Temporary arrays for dynamic programming over block counts. */
    seq_t *prev = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(N, sizeof(seq_t));
    if (!prev || !cur) {
        free(ans);
        abort();
    }
    prev[0] = (seq_t)1; /* F[1][0] = 1 */

    /* For each possible block count k ≥ 2 */
    for (size_t k = 2; k <= N; ++k) {
        /* Pre‑compute (k-1)^(g-1) for g = 1 … 9 (exponent ≤ 8). */
        seq_t pow_kminus1[9];
        pow_kminus1[0] = (seq_t)1;
        for (int e = 1; e <= 8; ++e) {
            pow_kminus1[e] = pow_kminus1[e - 1] * (seq_t)(k - 1);
        }

        /* Compute F[k][s] for s = k‑1 … N‑1. */
        memset(cur, 0, N * sizeof(seq_t));
        size_t min_s = k - 1;
        for (size_t s = min_s; s < N; ++s) {
            seq_t sum = (seq_t)0;
            for (int g = 1; g <= 9; ++g) {
                if (s < (size_t)g) break;
                seq_t term = prev[s - g];
                if (!term) continue;
                term = term * pow_kminus1[g - 1];
                sum += term;
            }
            cur[s] = sum;
        }

        /* Accumulate contributions of all partitions with exactly k blocks.
           We use the recurrence:
           ans_k(n) = k * ans_k(n-1) + F[k][n-1]   for n ≥ 2, ans_k(1)=0. */
        seq_t ans_k_prev = (seq_t)0;
        for (size_t npos = 2; npos <= N; ++npos) {
            seq_t a_factor = cur[npos - 1];
            seq_t ans_k_new = (seq_t)k * ans_k_prev + a_factor;
            ans[npos] += ans_k_new;
            ans_k_prev = ans_k_new;
        }

        /* Prepare for the next block count.  Swap prev and cur pointers. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Store the requested value and free temporary arrays. */
    seq_t result = ans[N];
    free(ans);
    free(prev);
    free(cur);
    return result;
}
