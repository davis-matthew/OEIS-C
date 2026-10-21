/*
 * A274773  a(n) = floor(sqrt(2*n-1) + 1/2) - abs(2*(n-1) - (floor(sqrt(2*n-1)
 *          + 1/2))^2) + 1.
 * https://oeis.org/A274773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor(sqrt(2*n-1) + 1/2) - abs( 2*(n-1) -
 *            (floor(sqrt(2*n-1) + 1/2))^2 ) + 1 Offset: 1 Return type: seq_t
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for 64‑bit unsigned integers.
   Returns ⌊√x⌋.  Uses binary search with unsigned __int128
   to avoid overflow when squaring mid.  */
static inline idx_t isqrt(idx_t x)
{
    idx_t low = 0, high = x, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Computes a(n) for n >= 1.  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* t = 2*n - 1 (no overflow in idx_t) */
    idx_t t = (idx_t)2 * n - (idx_t)1;

    /* m = floor(sqrt(t)) */
    idx_t m = isqrt(t);

    /* k = nearest integer to sqrt(t)
       If t <= m^2 + m   →   k = m
       else              →   k = m + 1   */
    unsigned __int128 m_sq = (unsigned __int128)m * m;
    idx_t k = (t <= m_sq + m) ? m : m + (idx_t)1;

    /* abs(2*(n-1) - k^2) */
    idx_t two_n_minus_2 = (idx_t)2 * (n - (idx_t)1);
    unsigned __int128 k_sq = (unsigned __int128)k * k;
    idx_t diff = (two_n_minus_2 >= (idx_t)k_sq)
                 ? two_n_minus_2 - (idx_t)k_sq
                 : (idx_t)k_sq - two_n_minus_2;

    /* a(n) = k - diff + 1 */
    idx_t result = k - diff + (idx_t)1;

    return (seq_t)result;
}
