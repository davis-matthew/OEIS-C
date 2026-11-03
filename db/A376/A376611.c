/*
 * A376611  Irregular triangle read by rows: T(n,k) is the largest divisor of
 *          binomial(n,k) which is less than n, with 0 <= k <= n.
 * https://oeis.org/A376611
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5251, u16 n <= 5251, u32 n <= 5251, u64 n <= 5251,
 *            u128 n <= 5251, big any n
 * Validated: standard, pass, 104 terms through n = 4194 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A376611: irregular triangle read by rows.
 * a(n) = largest divisor of binomial(row,row_k) that is < row,
 * where (row,k) is the position of the (n-2)‑th element of the
 * concatenated triangle starting with row 2.
 */

#include <stdbool.h>

/* Return true if d divides C(n,k); n,k <= 10000. */
static bool divides_binom(idx_t n, idx_t k, idx_t d)
{
    /* factor d */
    idx_t temp = d;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp_d = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp_d;
            }
            /* exponent of p in C(n,k) = e(n,p) - e(k,p) - e(n-k,p) */
            idx_t expC = 0;
            for (idx_t t = n; t; t /= p) expC += t / p;
            for (idx_t t = k; t; t /= p) expC -= t / p;
            for (idx_t t = n - k; t; t /= p) expC -= t / p;
            if (exp_d > expC) return false;
        }
    }
    /* remaining prime factor */
    if (temp > 1) {
        idx_t p = temp;
        idx_t exp_d = 1;
        idx_t expC = 0;
        for (idx_t t = n; t; t /= p) expC += t / p;
        for (idx_t t = k; t; t /= p) expC -= t / p;
        for (idx_t t = n - k; t; t /= p) expC -= t / p;
        if (exp_d > expC) return false;
    }
    return true;
}

/* largest divisor < n of binomial(n,k) */
static seq_t largest_divisor_of_binom_less_than_n(idx_t n, idx_t k)
{
    for (idx_t d = n - 1; d >= 1; --d) {
        if (divides_binom(n, k, d)) return (seq_t)d;
    }
    return (seq_t)1;          /* safety, should never be reached */
}

 

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below offset */

    /* map n to (row, col) in the triangle */
    idx_t idx = n - 2;   /* zero‑based position in concatenated rows */
    idx_t row = 2;
    while (idx >= row + 1) {     /* row has row+1 terms (k=0..row) */
        idx -= row + 1;
        ++row;
    }
    idx_t col = idx;                     /* 0 <= col <= row */

    return largest_divisor_of_binom_less_than_n(row, col);
}
