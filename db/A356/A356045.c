/*
 * A356045  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} sigma_k(j) * floor(n/j).
 * https://oeis.org/A356045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element on the antidiagonal of the square array
 *            T(n,k) = ∑_{j=1..n} σ_k(j) · ⌊n/j⌋, read by antidiagonals
 *            downwards. The index n maps to (i,j) with i = 1..s, j = s-i,
 *            where s is the smallest integer such that s(s+1)/2 ≥ n
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 175, u64 n <= 441,
 *            u128 n <= 1138, big any n
 * Validated: standard, pass, 82 terms through n = 9339 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

seq_t a(idx_t n)
{
    /* indices in the problem start at 1; ignore n==0 safely */
    if (n == 0) return 0;

    /* Find the diagonal number s (1‑based) so that triangular number
       t(s) = s(s+1)/2 just exceeds or equals n. */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n) {
        ++s;
    }

    /* Previous diagonal's total elements */
    idx_t prev = (s - 1) * s / 2;
    /* Zero‑based position inside the current diagonal */
    idx_t offset = n - prev - 1;

    /* Coordinates (i,j) in T: i runs from 1 up to s, j = s - i */
    idx_t i = offset + 1;
    idx_t j = s - i;   /* column index k */

    /* Compute T(i,j) = Σ_{t=1..i} σ_j(t) * ⌊i/t⌋ */
    seq_t total = 0;
    for (idx_t t = 1; t <= i; ++t) {
        seq_t sigma = sigma_k(t, (uint8_t)j);
        seq_t term  = sigma * (seq_t)(i / t);
        total += term;
    }
    return total;
}
