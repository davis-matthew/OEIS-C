/*
 * A328610  Irregular triangular array read by rows: the rows show the
 *          coefficients of the first of two factors of even-degree
 *          polynomials described in Comments.
 * https://oeis.org/A328610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 37, i16 n <= 108, i32 n <= 358, i64 n <= 1242,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A328610: Coefficients of the first factor f(k) in the
 * even‑degree polynomials described in the OEIS comments.
 *
 * For n = 1: a(1) = -2.
 * For n >= 2:
 *   Let k be the smallest integer with k(k+3)/2 >= n
 *   Let j = n - (k-1)(k+2)/2 - 1   (0‑based index in row k)
 *   If j == k: a(n) = 1
 *   Else:      a(n) = C(k, j) * Fib(k-j-1),
 *              where C(k, j) is the binomial coefficient
 *              and Fib(m) is the Fibonacci sequence (Fib(0)=0, Fib(1)=1).
 *
 * This implementation works for any seq_t and idx_t provided
 * by the build system.  It uses only integer arithmetic and
 * guarantees exactness for all terms that fit in seq_t.
 */

#include <stdint.h>

/* Helper: Fibonacci number (non‑recursive, iterative) */
static seq_t
fib(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a = (seq_t)0, b = (seq_t)1, c;
    for (idx_t i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

/* Helper: Binomial coefficient C(n, k) using __int128 to avoid overflow */
static seq_t
binom(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        __int128 temp = (__int128)res * (n - k + i);
        temp /= i;
        res = (seq_t)temp;
    }
    return res;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)-2;

    /* Find smallest k with k(k+3)/2 >= n */
    idx_t k = (idx_t)1;
    while (k * (k + (idx_t)3) / (idx_t)2 < n) {
        ++k;
    }

    idx_t prev = (k - (idx_t)1) * (k + (idx_t)2) / (idx_t)2;
    idx_t j = n - prev - (idx_t)1;

    if (j == k) return (seq_t)1;

    seq_t comb = binom(k, j);
    idx_t fib_idx = k - j - (idx_t)1;
    seq_t fib_val = fib(fib_idx);

    return comb * fib_val;
}
