/*
 * A368380  Arises from enumeration of a certain class of partial zig-zag
 *          knight's paths on the square grid.
 * https://oeis.org/A368380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (1/x + 1 + 2*R + R^2) * R^3 + R^2 /
 *            x where R(x) = (1 - sqrt(1-4*x^2))/(2*x^2) - 1. This is
 *            equivalent to a(2j-1) = s3[j] + s2[j] (odd n) a(2j) = s3[j] +
 *            2*s4[j] + s5[j] (even n) with s_m[j] = coefficient of x^(2j) in
 *            R(x)^m
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: All libutils functions used by the build system should be
   available.  We only use malloc/free helpers. */
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* Cached coefficient sequences */
static seq_t *t  = NULL; /* t[k] = Catalan(k) for k>=1   */
static seq_t *s2 = NULL; /* coefficients of R^2          */
static seq_t *s3 = NULL; /* coefficients of R^3          */
static seq_t *s4 = NULL; /* coefficients of R^4          */
static seq_t *s5 = NULL; /* coefficients of R^5          */
static size_t max_K = 0; /* maximum index currently stored */

/* Allocate or resize cached tables for all k up to K = n/2 + 1. */
static void compute_tables(idx_t n)
{
    size_t K = (n >> 1) + 1;          /* ceil((n+1)/2) */
    if (K <= max_K) return;          /* already ready */

    /* free old tables if any */
    if (t)  free_seq(t);
    if (s2) free_seq(s2);
    if (s3) free_seq(s3);
    if (s4) free_seq(s4);
    if (s5) free_seq(s5);

    /* allocate new arrays of size K+1 (index 0 unused) */
    t  = alloc_seq(K + 1);
    s2 = alloc_seq(K + 1);
    s3 = alloc_seq(K + 1);
    s4 = alloc_seq(K + 1);
    s5 = alloc_seq(K + 1);

    /* initialise to zero */
    for (size_t i = 0; i <= K; ++i) {
        t[i] = s2[i] = s3[i] = s4[i] = s5[i] = 0;
    }

    /* Catalan numbers: Cat(0)=1, Cat(k+1)=Cat(k)*(4k+2)/(k+2) */
    t[1] = 1;                          /* Cat(1) */
    for (size_t k = 1; k < K; ++k) {
        /* t[k+1] = t[k] * (4k+2) / (k+2) */
        seq_t numerator = t[k] * (seq_t)(4 * k + 2);
        t[k + 1] = numerator / (k + 2);
    }

    /* s2[k] = Σ_{i=1}^{k-1} t[i]*t[k-i]   for k>=2 */
    for (size_t k = 2; k <= K; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i < k; ++i) {
            sum += t[i] * t[k - i];
        }
        s2[k] = sum;
    }

    /* General convolution: s_{m+1}[k] = Σ_{i=1}^{k-1} s_m[i]*t[k-i] */
    /* s3 */
    for (size_t k = 2; k <= K; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i < k; ++i) {
            sum += s2[i] * t[k - i];
        }
        s3[k] = sum;
    }
    /* s4 */
    for (size_t k = 2; k <= K; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i < k; ++i) {
            sum += s3[i] * t[k - i];
        }
        s4[k] = sum;
    }
    /* s5 */
    for (size_t k = 2; k <= K; ++k) {
        seq_t sum = 0;
        for (size_t i = 1; i < k; ++i) {
            sum += s4[i] * t[k - i];
        }
        s5[k] = sum;
    }

    max_K = K;
}

seq_t a(idx_t n)
{
    /* values for n=0,1,2 are 0 */
    if (n < 3) return 0;

    compute_tables(n);

    if ((n & 1) == 0) {               /* n even : a(2j)   */
        size_t j = (size_t)(n >> 1);
        seq_t res = 0;
        if (j <= max_K) {
            res = s3[j] + 2 * s4[j] + s5[j];
        }
        return res;
    } else {                           /* n odd  : a(2j-1) */
        size_t j = (size_t)((n + 1) >> 1);
        seq_t res = 0;
        if (j <= max_K) {
            res = s3[j] + s2[j];
        }
        return res;
    }
}
