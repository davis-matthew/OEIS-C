/*
 * A389648  Y coordinate of point n while traversing the half plane by integer
 *          points in rectangular layers starting from n=1 at the origin.
 * https://oeis.org/A389648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n):  Y‑coordinate of the n‑th point while traversing the half‑plane
 * y ≥ 0 by rectangular layers centered at the origin.  For layer k
 * the cumulative count of points satisfies S(k) = 1 + 2k² + 3k.
 * For a given n we locate its layer k (S(k‑1) < n ≤ S(k)), find the
 * offset p = n – S(k‑1) – 1 inside that layer (0 ≤ p ≤ 4k), and then
 * the y coordinate is:
 *   if p ≤ k          y = p
 *   else if p ≤ 3k    y = k
 *   else              y = k – (p – 3k)
 *
 * This implementation works for any seq_t and idx_t supplied by the
 * build environment, using only integer arithmetic. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Index 1 is the origin (0,0). */
    if (n == 1)
        return 0;

    /* Find the smallest k ≥ 1 such that S(k) ≥ n. */
    idx_t low = 1, high = 1;
    while (1) {
        unsigned __int128 s = 1 + (__int128)2 * high * high + (__int128)3 * high;
        if (s >= n)
            break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = 1 + (__int128)2 * mid * mid + (__int128)3 * mid;
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;          /* layer number (k ≥ 1) */

    /* Start index of this layer: S(k‑1) + 1. */
    idx_t km1 = k - 1;
    unsigned __int128 start_s = 1 + (__int128)2 * km1 * km1 + (__int128)3 * km1;
    idx_t start = (idx_t)start_s + 1;

    idx_t p = n - start;    /* 0‑based offset within the layer [0, 4k] */

    seq_t y;
    if (p <= k)
        y = (seq_t)p;
    else if (p <= 3 * k)
        y = (seq_t)k;
    else
        y = (seq_t)(k - (p - 3 * k));

    return y;
}
