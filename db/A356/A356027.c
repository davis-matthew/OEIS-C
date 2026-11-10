/*
 * A356027  Duplicate of A101391.
 * https://oeis.org/A356027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   T(n,k) = Sum_{d|n} binomial(d-1,k-1)*mobius(n/d), read by rows
 *            (A101391); a(n) = T(r,k) with r = row, k = column
 * Range:     i8 n <= 39, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   mobius_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mobius_idx.h"

static idx_t A101391_row_of(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low = high;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t A101391_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t r = A101391_row_of(n - 1) + 1;
    idx_t prv = (r - 1) * r / 2;
    idx_t k = n - prv;

    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d != 0) continue;
        idx_t d2 = r / d;
        if (k <= d) {
            int mu = mobius_idx(d2);
            if (mu != 0) {
                seq_t b = A101391_binom(d - 1, k - 1);
                if (mu > 0) pos += b; else neg += b;
            }
        }
        if (d2 != d && k <= d2) {
            int mu = mobius_idx(d);
            if (mu != 0) {
                seq_t b = A101391_binom(d2 - 1, k - 1);
                if (mu > 0) pos += b; else neg += b;
            }
        }
    }
    return pos - neg;
}
