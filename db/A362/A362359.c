/*
 * A362359  Triangle T read by rows, obtained from the array A for the
 *          solutions of the Monkey and Coconuts Problem (s sailors and one
 *          coconut to the monkey).
 * https://oeis.org/A362359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n,
 *            k = n − r(r−1)/2, and T(r,k) = (r−k+1)·k^(k+1) − (k−1) for 1 ≤ k
 *            ≤ r
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 53, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for off‑by‑one callers */

    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while (true) {
        __int128 tri = (__int128)r * (r + 1) / 2;
        if (tri >= n) break;
        ++r;
    }

    /* k is the position within the row (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;

    /* Compute (r-k+1) * k^(k+1) - (k-1) */
    seq_t kk          = (seq_t)k;
    seq_t mult_factor = (seq_t)(r - k + 1);
    seq_t powk        = big_pow(kk, (seq_t)(k + 1));
    seq_t val         = mult_factor * powk;
    val -= (seq_t)(k - 1);        /* subtract (k-1) */

    return val;
}
