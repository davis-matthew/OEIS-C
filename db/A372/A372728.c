/*
 * A372728  Kronecker's triangle read by rows. T(n, k) = K(n, k) where K(n, k)
 *          is the Kronecker symbol (n / k).
 * https://oeis.org/A372728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A372728 – Kronecker's triangle read by rows.
 *
 *  Each element of the triangle is K(n,k) = (n/k), the Kronecker
 *  symbol of n modulo k.  a(n) returns the nth element of the
 *  triangle read row‑wise, offset 0.
 *
 *  a(n) is computed by
 *      1. determining the row r and column j where the index lies
 *         (r(r+1)/2 ≤ n < (r+1)(r+2)/2,  j = n – r(r+1)/2);
 *      2. evaluating the Kronecker symbol (r / j).  The symbol
 *         is obtained by peeling off powers of two from the
 *         denominator, handling the (a/2) part, and then
 *         computing the Jacobi symbol for the remaining odd part
 *         (using the provided jacobi_symbol routine).
 *
 *  No memoisation is required; the algorithm is O(1) per call.
 *  The implementation uses only integer arithmetic and works for
 *  arbitrary precision seq_t or any 64‑bit signed integer.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

/* ---- auxiliary: compute the Kronecker symbol (a / n) for n > 0 ---- */
static inline seq_t
kronecker_uint64(uint64_t a, uint64_t n)
{
    /* Special cases for the denominator 0/1. */
    if (n == 0U) {
        return (a == 1U ? 1 : 0);
    }
    /* (0 / n) = 0 for any n > 1, and (0 / 1) = 0. */
    if (a == 0U) {
        return 0;
    }

    /* Factor powers of two out of the denominator. */
    unsigned e = 0;
    while ((n & 1ULL) == 0U) {
        e++;
        n >>= 1;
    }

    int part = 1;   /* product of the (a/2) factors. */
    if (e > 0) {
        if ((a & 1ULL) == 0U) {   /* (even / 2) = 0 */
            return 0;
        }
        uint64_t mod8 = a & 7ULL;
        int t = (mod8 == 1ULL || mod8 == 7ULL) ? 1 : -1;
        if (e & 1U) {
            part = t;   /* odd number of (a/2) factors */
        }
    }

    /* If the remaining odd part is 1, the Jacobi symbol is 1. */
    if (n == 1ULL) {
        return (seq_t)part;
    }

    /* Compute the Jacobi symbol (a / n).  */
    int jv = jacobi_symbol(a, n);    /* -1, 0 or 1 */
    if (jv == 0) {
        return 0;
    }
    return (seq_t)(part * jv);
}

/* ---- auxiliary: find the row for a given index n ---- */
static inline idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1U << 32);   /* wide upper bound */
    while (low < high) {
        idx_t mid = low + ((high - low + 1U) >> 1U);
        __uint128_t tri = (__uint128_t)mid * (mid + 1U) / 2U;
        if (tri <= n) {
            low = mid;
        } else {
            high = mid - 1U;
        }
    }
    return low;
}

/* ---- required public function: a(n) ---- */
seq_t a(idx_t n)
{
    /* Determine the row r and column j for the index n. */
    idx_t r = find_row(n);
    __uint128_t off = (__uint128_t)r * (r + 1U) / 2U;
    idx_t j = n - (idx_t)off;

    return kronecker_uint64((uint64_t)r, (uint64_t)j);
}
