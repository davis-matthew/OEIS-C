/*
 * A317360  Triangle a(n, k) read by rows: coefficient triangle that gives
 *          Lucas powers and sums of Lucas powers.
 * https://oeis.org/A317360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n, k) = Sum_{j=0..k} Lucas(k+1-j)^n * A055870(n+1, j), with
 *            A055870(r, m) = (-1)^floor((m+1)/2) * fibonomial(r, m)
 * Range:     i8 n <= 11, i16 n <= 30, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A317360_fib(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

static seq_t A317360_lucas(idx_t n) {
    if (n == 0) return (seq_t)2;
    seq_t l0 = 2, l1 = 1, t;
    for (idx_t i = 2; i <= n; ++i) {
        t = l0 + l1;
        l0 = l1;
        l1 = t;
    }
    return l1;
}

static seq_t A317360_pow(seq_t b, idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= b;
    return r;
}

static seq_t A317360_fibonomial(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t top = A317360_fib(n + 1 - i);
        seq_t bottom = A317360_fib(i);
        seq_t g = gcd(top, bottom);
        if (g > (seq_t)1) {
            top /= g;
            bottom /= g;
        }
        seq_t g2 = gcd(res, bottom);
        if (g2 > (seq_t)1) {
            res /= g2;
            bottom /= g2;
        }
        res = res * top;
        res = res / bottom;
    }
    return res;
}

/* signed Fibonomial A055870(r, m) */
static seq_t A317360_signed_fibo(idx_t r, idx_t m)
{
    seq_t fn = A317360_fibonomial(r, m);
    idx_t e = (m + 1) >> 1;
    return (e & 1) ? -fn : fn;
}

seq_t a(idx_t n)
{
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n) ++row;
    idx_t k = n - row * (row + 1) / 2;
    idx_t N = row;

    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t L = A317360_lucas(k + 1 - j);
        seq_t p = A317360_pow(L, N);
        sum += p * A317360_signed_fibo(N + 1, j);
    }
    return sum;
}
