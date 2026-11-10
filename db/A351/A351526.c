/*
 * A351526  Expansion of e.g.f. (log(1 + log(1 + log(1 + log(1+ x)))))^2 / 2.
 * https://oeis.org/A351526
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (1/2) * Sum_{k=1..n-1} binomial(n,k) *
 *            A000310(k) * A000310(n-k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 18 (2026-11-10)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

#define A310_CAP 20001

static seq_t A000310_F[4][A310_CAP];
static seq_t A000310_R[A310_CAP];
static idx_t A000310_N = 0;

static seq_t A000310_a(idx_t n)
{
    if (n < 1 || n >= A310_CAP)
        return 0;
    while (A000310_N < n) {
        idx_t m = A000310_N + 1;
        idx_t k;
        int lev;
        if (m == 1) {
            A000310_R[0] = 1;
        } else {
            A000310_R[m - 1] = 1;
            for (k = m - 2; k >= 1; k--)
                A000310_R[k] += A000310_R[k - 1];
        }
        if (m == 1)
            A000310_F[0][1] = 1;
        else
            A000310_F[0][m] = A000310_F[0][m - 1] * (seq_t)(m - 1);
        for (lev = 1; lev <= 3; lev++) {
            seq_t s = A000310_F[lev - 1][m];
            for (k = 1; k < m; k++)
                s += A000310_R[k] * A000310_F[lev - 1][k] * A000310_F[lev][m - k];
            A000310_F[lev][m] = s;
        }
        A000310_N = m;
    }
    return A000310_F[3][n];
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    if (n < 2)
        return 0;
    for (k = 1; 2 * k < n; k++)
        s += binomial(n, k) * A000310_a(k) * A000310_a(n - k);
    if ((n & 1) == 0) {
        seq_t h = A000310_a(n / 2);
        s += (binomial(n, n / 2) / 2) * h * h;
    }
    if (n & 1)
        s = -s;
    return s;
}
