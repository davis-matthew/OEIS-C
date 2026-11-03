/*
 * A372877  Jacobi's triangle read by rows. T(n, k) = J(n, k) where J(n, k) is
 *          the Jacobi symbol (n / k) provided k is odd and otherwise 0.
 * https://oeis.org/A372877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = J(r,k) where r and k satisfy n = r*(r+1)/2 + k, 0 ≤ k ≤
 *            r. J(r,k) is the Jacobi symbol (r/k) for odd k; if k is even or
 *            not coprime to r, J(r,k) = 0
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       For n up to ~10^5 a simple loop is fast enough. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* The starting index of this row */
    idx_t start = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t k = n - start;

    /* If k is even, Jacobi symbol is defined as 0. */
    if ((k & 1ULL) == 0)
        return (seq_t)0;

    /* Special case: k == 1 → Jacobi symbol (r/1) = 1 always. */
    if (k == 1)
        return (seq_t)1;

    /* Compute the Jacobi symbol (r/k).  r is non‑negative by construction. */
    int s = jacobi_symbol((uint64_t)r, (uint64_t)k);
    return (seq_t)s;
}
