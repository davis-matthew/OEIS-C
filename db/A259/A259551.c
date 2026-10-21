/*
 * A259551  Expansion of f(x^2, x^3) * f(-x^4, -x^6) / f(-x^2) in powers of x
 *          where f(,) is the Ramanujan general theta function.
 * https://oeis.org/A259551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 170, u32 n <= 539, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in F(x),
 * where   F(x) = f(x^2 ,x^3) * f(-x^4 , -x^6) / f(-x^2)
 * f(a,b) = Σ_{k∈ℤ} a^{k(k+1)/2} b^{k(k-1)/2}
 * f(-x^2) = Σ_{m∈ℤ} (-1)^m x^{m(3m-1)}
 *
 * The implementation builds the generating functions A,B,C up to the
 * requested index, convolves A and B to obtain D, then performs a
 * power‑series division to obtain the desired coefficient.
 */

#include <stdlib.h>
#include <stdint.h>

/* memoised coefficient arrays */
static long long *A = NULL;   /* coefficients of f(x^2 ,x^3)          */
static long long *B = NULL;   /* coefficients of f(-x^4 , -x^6)        */
static long long *C = NULL;   /* coefficients of f(-x^2)               */
static long long *D = NULL;   /* convolution A*B                        */
static long long *R = NULL;   /* resulting series a(n)                 */
static idx_t computed = 0;    /* largest n that has been processed     */

/* extend the memoised tables up to and including n */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= computed) return;            /* already available */

    size_t newsize = (size_t)(n + 1);
    /* reallocate each array */
    A = (long long *)realloc(A, newsize * sizeof(long long));
    B = (long long *)realloc(B, newsize * sizeof(long long));
    C = (long long *)realloc(C, newsize * sizeof(long long));
    D = (long long *)realloc(D, newsize * sizeof(long long));
    R = (long long *)realloc(R, newsize * sizeof(long long));

    /* initialise all entries to 0 */
    for (size_t i = 0; i < newsize; ++i) {
        A[i] = B[i] = C[i] = D[i] = R[i] = 0;
    }

    /* --------------------------------------------------------------------
     *  coefficients of f(x^2 ,x^3)    (series A)
     *  exponent e = (5k^2 - k)/2   for k > 0
     *  exponent e = (5k^2 + k)/2   for k < 0
     * -------------------------------------------------------------------- */
    for (idx_t k = 0;; ++k) {
        long long e_pos = (5LL * k * k - k) / 2;
        if (e_pos > n) break;               /* higher k will only grow exponents */
        if (k == 0) {
            A[0] = 1;
        } else {
            A[e_pos] += 1;
            long long e_neg = (5LL * k * k + k) / 2;
            if (e_neg <= n) A[e_neg] += 1;
        }
    }

    /* --------------------------------------------------------------------
     *  coefficients of f(-x^4 , -x^6)   (series B)
     *  exponent e = 5k^2 - k   for k > 0
     *  exponent e = 5k^2 + k   for k < 0
     *  sign = (-1)^k
     * -------------------------------------------------------------------- */
    for (idx_t k = 0;; ++k) {
        long long e_pos = 5LL * k * k - k;
        if (e_pos > n) break;
        if (k == 0) {
            B[0] = 1;
        } else {
            long long sign = (k & 1) ? -1LL : 1LL;
            B[e_pos] += sign;
            long long e_neg = 5LL * k * k + k;
            if (e_neg <= n) B[e_neg] += sign;
        }
    }

    /* --------------------------------------------------------------------
     *  coefficients of f(-x^2)           (series C)
     *  exponent e = m(3m-1)   for m > 0
     *  exponent e = m(3m+1)   for m < 0 (m = -k)
     *  sign   = (-1)^m
     * -------------------------------------------------------------------- */
    for (idx_t m = 0;; ++m) {
        long long e0 = m * (3LL * m - 1LL);
        if (e0 > n) break;
        if (m == 0) {
            C[0] = 1;
        } else {
            long long sign = (m & 1) ? -1LL : 1LL;
            C[e0] += sign;                     /* m > 0 */
            long long e1 = m * (3LL * m + 1LL);  /* m < 0 */
            if (e1 <= n) C[e1] += sign;
        }
    }

    /* --------------------------------------------------------------------
     *  convolution D = A * B
     * -------------------------------------------------------------------- */
    for (idx_t i = 0; i <= n; ++i) {
        if (!A[i]) continue;
        for (idx_t j = 0; i + j <= n; ++j) {
            if (!B[j]) continue;
            D[i + j] += A[i] * B[j];
        }
    }

    /* --------------------------------------------------------------------
     *  division:   D = C * R   =>   R[0] = D[0],
     *  R[m] = D[m] - Σ_{k=1..m} R[m-k] * C[k]
     * -------------------------------------------------------------------- */
    R[0] = D[0];
    for (idx_t m = 1; m <= n; ++m) {
        long long sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            if (!C[k]) continue;
            sum += R[m - k] * C[k];
        }
        R[m] = D[m] - sum;
    }

    computed = newsize;
}

/* public interface required by the build system */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return (seq_t)R[n];
}
