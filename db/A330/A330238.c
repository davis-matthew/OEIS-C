/*
 * A330238  Triangle T(n,k): concatenate the absolute differences of the
 *          digits of n and k (the smaller one padded with leading zeros); n
 *          >= k >= 1.
 * https://oeis.org/A330238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = T(r,k) where 1 ≤ k ≤ r and (r-1)*r/2 < n ≤ r*(r+1)/2;
 *            T(r,k) is the concatenation of the absolute differences of the
 *            decimal digits of r and k, padding the shorter number with
 *            leading zeros
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition, but guard against an accidental 0 */
    if (n == 0) return 0;

    /* find the row r such that the cumulative count of entries up to
       row r-1 is less than n, while the count up to row r is at least n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* k is the column number within that row (1‑based) */
    idx_t prev_tri = (r * (r - 1)) / 2;   /* (r-1)*r/2 */
    idx_t k = n - prev_tri;               /* 1 ≤ k ≤ r */

    /* Extract decimal digits of r and k (least significant first) */
    uint8_t dr[20] = {0}, dk[20] = {0};
    size_t lr = 0, lk = 0;
    idx_t tmp = r;
    do {
        dr[lr++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);
    tmp = k;
    do {
        dk[lk++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* Pad the shorter number with leading zeros: length = max(lr, lk) */
    size_t len = (lr > lk) ? lr : lk;

    /* Build most‑significant‑first digit arrays for r and k */
    uint8_t ar[20] = {0}, ak[20] = {0};
    for (size_t i = 0; i < lr; ++i) {
        ar[len - 1 - i] = dr[i];
    }
    for (size_t i = 0; i < lk; ++i) {
        ak[len - 1 - i] = dk[i];
    }

    /* Concatenate the absolute digit differences */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d1 = ar[i];
        uint8_t d2 = ak[i];
        uint8_t diff = (d1 > d2) ? (d1 - d2) : (d2 - d1);
        res = res * 10 + (seq_t)diff;
    }

    return res;
}
