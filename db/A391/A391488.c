/*
 * A391488  Triangle read by rows: T(n,k) is the number of endofunctions on
 *          [n] where the difference between the smallest and largest values
 *          in the image of the function is k, 0 <= k < n.
 * https://oeis.org/A391488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 61, u64 n <= 148,
 *            u128 n <= 400, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th entry of the triangle T(r,k) read row‑wise.
 * For each n >= 1:
 *   Find the row r such that r*(r+1)/2 >= n, 
 *   Let prev = (r-1)*r/2 and k = n - prev - 1.
 *   Then
 *     if (k == 0)       a(n) = r;
 *     else              a(n) = (r-k)*[(k+1)^r - 2*k^r + (k-1)^r].
 */
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t N) {
    /* Find the row r */
    idx_t r = 1;
    while (((idx_t)r * (r + 1)) / 2 < N)
        ++r;

    idx_t prev = ((idx_t)(r - 1) * r) / 2;
    idx_t k = N - prev - 1;

    if (k == 0) {
        return (seq_t)r;
    } else {
        seq_t p1 = big_pow((seq_t)(k + 1), (seq_t)r);
        seq_t p2 = big_pow((seq_t)k, (seq_t)r);
        seq_t p3 = big_pow((seq_t)(k - 1), (seq_t)r);

        /* Compute e = p1 + p3 - 2*p2 */
        seq_t e = p1 + p3;
        seq_t t = p2;
        t <<= 1;          /* multiply by 2 */
        e -= t;

        seq_t result = ((seq_t)(r - k)) * e;
        return result;
    }
}
