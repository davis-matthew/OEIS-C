/*
 * A286247  Triangular table: T(n,k) = 0 if k does not divide n, otherwise
 *          T(n,k) = P(A046523(k), n/k), where P is sequence A000027 used as a
 *          pairing function N x N -> N. Table is read by rows as T(1,1),
 *          T(2,1), T(2,2), etc.
 * https://oeis.org/A286247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangular table T(n,k) where T(n,k) = 0 if k does not
 *            divide n, otherwise T(n,k) = P(A046523(k), n/k), P is Cantor's
 *            pairing function A046523(k) is the minimal integer with the same
 *            prime signature as k
 * Range:     u8 n <= 276, u16 n <= 9431, u32 n <= 9431, u64 n <= 9431,
 *            u128 n <= 9431, big any n
 * Helpers:   a046523
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a046523.h"

/* --------- Helper: compute A046523(k) --------- */

/* --------- Helper: find row r such that r*(r+1)/2 >= n --------- */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* --------- Main function: OEIS A286247 --------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1, but guard */

    /* Determine (row r, column k) in the triangular representation. */
    idx_t r = find_row(n);                   /* row number (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;        /* sum of lengths of previous rows */
    idx_t k = n - prev_tri;                  /* column number (1‑based) */

    /* If k does not divide r, the entry is 0. */
    if (r % k != 0) return (seq_t)0;

    idx_t q = r / k;                         /* quotient n/k                       */
    seq_t sig = a046523(k);                  /* A046523(k)                          */

    /* Compute the Cantor pairing function P(sig, q):
       P(a,b) = (a+b-1)(a+b-2)/2 + a
       Using the algebraic form given in the OEIS entry:
       P = (1/2)*(2 + (sig+q)^2 - sig - 3*q) */
    seq_t sum = sig + (seq_t)q;
    seq_t val = (sum * sum + (seq_t)2 - sig - (seq_t)3 * q) / (seq_t)2;

    return val;
}
