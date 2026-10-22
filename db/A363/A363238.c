/*
 * A363238  Number of partitions of n with rank a multiple of 6.
 * https://oeis.org/A363238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n with rank a multiple of 6.
 *            Computation uses the generating function A(x) =
 *            (1/Π_{k≥1}(1-x^k)) * Σ_{k≥1} (-1)^{k-1} x^{k(3k-1)/2}
 *            (1-x^k)(1+x^{6k})/(1-x^{6k}). The sequence is obtained by
 *            expanding the product and taking the coefficient of x^n. The
 *            implementation c...
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

static seq_t *a_arr = NULL;    /* cache of a[0..curN] */
static idx_t curN   = 0;       /* highest index for which a_arr is valid */

/* Recompute a[0..N] when a larger index is requested.  The computation
   is performed from scratch; for n up to 10 000 this takes well under
   the required 2 seconds. */
static void recompute_to(idx_t N)
{
    if (N <= curN) return;

    /* Allocate temporary arrays */
    seq_t *p = (seq_t*)malloc((N + 1) * sizeof(seq_t));          /* partitions p[0..N]   */
    long long *q = (long long*)calloc((N + 1), sizeof(long long));/* Q(x) coefficients   */
    seq_t *pos = (seq_t*)calloc((N + 1), sizeof(seq_t));        /* positive part of a   */
    seq_t *neg = (seq_t*)calloc((N + 1), sizeof(seq_t));        /* negative part of a   */

    /* 1. Compute partition numbers p[0..N] via Euler's pentagonal recurrence.
       The recurrence uses only additions and subtractions on unsigned values
       by keeping positive and negative contributions separately. */
    p[0] = 1;
    for (idx_t n = 1; n <= N; n++) {
        seq_t sum_pos = 0, sum_neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            if (k & 1) sum_pos += p[n - pent1];
            else       sum_neg += p[n - pent1];
        }
        for (idx_t k = 1;; k++) {
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > n) break;
            if (k & 1) sum_pos += p[n - pent2];
            else       sum_neg += p[n - pent2];
        }
        p[n] = sum_pos - sum_neg;  /* p[n] is always non‑negative   */
    }

    /* 2. Compute Q(x) coefficients q[0..N] from its definition.
       Only small integers (±1, ±2) appear as coefficients. */
    for (idx_t k = 1;; k++) {
        idx_t pent = k * (3 * k - 1) / 2;
        if (pent > N) break;
        long long sign = (k & 1) ? 1 : -1;   /* (-1)^(k-1) */
        idx_t t = 0;
        for (;; t++) {
            idx_t e = pent + 6 * k * t;
            if (e > N) break;
            long long coeff = (t == 0) ? sign : sign * 2;   /* in the plus part */
            q[e] += coeff;
            idx_t e3 = e + k;
            if (e3 <= N) q[e3] -= sign;
            idx_t e4 = e + 7 * k;
            if (e4 <= N) q[e4] -= sign;
        }
    }

    /* 3. Convolution A(x) = P(x) * Q(x).
       Accumulate positive and negative contributions separately to avoid
       intermediate underflows. */
    for (idx_t i = 0; i <= N; i++) {
        for (idx_t j = 0; j <= N - i; j++) {
            long long coeff = q[j];
            if (coeff == 0) continue;
            seq_t term = mul_small(p[i], (unsigned int)(coeff > 0 ? coeff : -coeff));
            idx_t k = i + j;
            if (coeff > 0) pos[k] += term;
            else           neg[k] += term;
        }
    }

    /* 4. Finalise a[0..N] from the accumulated parts. */
    seq_t *new_a = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    for (idx_t k = 0; k <= N; k++)
        new_a[k] = pos[k] - neg[k];

    /* 5. Release temporary storage and update cache. */
    free(p);
    free(q);
    free(pos);
    free(neg);
    if (a_arr) free(a_arr);
    a_arr = new_a;
    curN  = N;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; a(0) is undefined */
    recompute_to(n);                  /* ensure a_arr covers the required index */
    return a_arr[n];
}
