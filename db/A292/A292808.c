/*
 * A292808  G.f.: Sum_{n>=1} Sum_{k=0..n} binomial(n,k) * x^(k^2) * (x^n -
 *          x^k)^(n-k), ignoring the constant term.
 * https://oeis.org/A292808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 17, i16 n <= 66, i32 n <= 222, i64 n <= 520,
 *            i128 n <= 520, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the coefficient of x^n in the power series
 *   A(x) = Σ_{N≥1} Σ_{k=0..N} binom(N,k) * x^{N*k} * (x^{N-k}-1)^{N-k}
 *   (ignoring the constant term).
 *   This is equivalent to
 *     a(n) = Σ_{N=1..n}  Σ_{k=0..N} binom(N,k) * binom(N-k,t) * (-1)^{N-k-t}
 *     where t = (n - N*k)/(N-k) is an integer with 0 ≤ t ≤ N-k.
 *   Additionally, if n = N^2 we add the term binom(N,N)=1.
 *
 * Implementation performs a direct summation over N and k,
 * using the supplied binomial helper.  The inner check reduces
 * the number of expensive computations greatly.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t N = 1; N <= n; ++N) {
        /* k = N case : contributes when n == N^2 */
        if (n == (N * N)) {
            result += (seq_t)1;
        }

        for (idx_t k = 0; k < N; ++k) {
            idx_t Nk = N * k;
            if (Nk > n) break;          /* Further k will only increase Nk */
            idx_t denom = N - k;        /* N-k > 0 here */
            idx_t num   = n - Nk;

            /* t must be integer */
            if (num % denom != 0) continue;
            idx_t t = num / denom;
            if (t > denom) continue;    /* safety, though implied by num <= denom*denom */

            seq_t bin1 = binomial(N, k);            /* binom(N,k)   */
            seq_t bin2 = binomial(denom, t);        /* binom(N-k,t) */

            /* sign = (-1)^(denom - t) */
            seq_t sign = ((denom - t) & 1) ? (seq_t)-1 : (seq_t)1;

            result += bin1 * bin2 * sign;
        }
    }
    return result;
}
