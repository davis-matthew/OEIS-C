/*
 * A332753  G.f. A(x) satisfies: A(x) = x * (1 + A(x) + A(x^2)^2 + A(x^3)^3 +
 *          ...).
 * https://oeis.org/A332753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   new_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   For n ≥ 2,
       a(n) = a(n-1) + Σ_{k | (n-1), k ≥ 2} C_k((n-1)/k)
   with
       C_1(t) = a(t)
       C_k(t) = Σ_{j=1}^{t-1} a(j) * C_{k-1}(t-j)   (k ≥ 2) */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/new_seq_arr.h"

#if defined(__cplusplus)

#else

#endif

static idx_t     max_n = 0;     /* largest n that has been built */
static seq_t*    a_seq = NULL;  /* a_seq[1 .. max_n] (0 unused)     */
static seq_t**   conv  = NULL;  /* conv[k] for k = 2 .. max_n-1      */
static idx_t*    tmax  = NULL;  /* tmax[k] = highest t index in conv[k] */

/* Build all values up to N (inclusive). */
static void recompute_to(idx_t N)
{
    if (N <= max_n) return;          /* already up to date */

    /* Allocate new storage.  Old storage is left untouched;
       the total number of allocations grows at most once. */
    max_n = N;

    /* a_seq[0 .. max_n] */
    a_seq = new_seq_arr(max_n + 1);
    a_seq[0] = (seq_t)0;
    a_seq[1] = (seq_t)1;

    /* conv[0] and conv[1] are unused */
    conv = (seq_t**)calloc(max_n, sizeof(seq_t*));
    tmax = (idx_t*)calloc(max_n, sizeof(idx_t));

    /* Allocate conv[k] for k = 2 .. max_n-1 */
    for (idx_t k = 2; k < max_n; ++k) {
        idx_t size = (max_n - 1) / k + 1;   /* number of t‑s <= floor((max_n-1)/k) + 1 */
        conv[k] = new_seq_arr(size);
        tmax[k] = size - 1;                /* last valid index */
    }

    /* DP over n = 2 .. max_n */
    for (idx_t n = 2; n <= max_n; ++n) {
        idx_t m = n - 1;          /* m = n-1 */
        seq_t sum = a_seq[m];     /* a(m) term */

        /* Add contributions from all divisors k ≥ 2 of m */
        idx_t lim = (idx_t)floor(sqrt((double)m));
        for (idx_t d = 2; d <= lim; ++d) {
            if (m % d == 0) {
                idx_t k1 = d;
                sum += conv[k1][m / k1];
                idx_t e = m / d;
                if (e >= 2 && e != k1) {
                    sum += conv[e][m / e];
                }
            }
        }
        if (m >= 2) {
            sum += conv[m][1];   /* divisor k = m itself */
        }

        a_seq[n] = sum;

        /* Update convolution tables for future use */
        idx_t k_max = (max_n - 1) / n;  /* k such that n ≤ floor((max_n-1)/k) */
        for (idx_t k = 2; k <= k_max; ++k) {
            seq_t val = (seq_t)0;
            if (k == 2) {
                /* C_2(n) = Σ_{j=1}^{n-1} a(j) * a(n-j) */
                for (idx_t j = 1; j < n; ++j) {
                    val += a_seq[j] * a_seq[n - j];
                }
            } else {
                /* C_k(n) = Σ_{j=1}^{n-1} a(j) * C_{k-1}(n-j) */
                for (idx_t j = 1; j < n; ++j) {
                    val += a_seq[j] * conv[k - 1][n - j];
                }
            }
            conv[k][n] = val;
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */
    recompute_to(n);
    return a_seq[n];
}
