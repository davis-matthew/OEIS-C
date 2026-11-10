/*
 * A284384  Alternate A167381(n), A167381(n) + 1.
 * https://oeis.org/A284384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A167381(floor(n/2)) + (n mod 2), with A167381(0) = 0
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t A167381_find_k(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

static seq_t A167381_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t k = A167381_find_k(n);
    idx_t prev_cum = (k - 1) * k;
    idx_t t = n - prev_cum - 1;

    seq_t km1 = (seq_t)(k - 1);
    seq_t kk = (seq_t)k;
    seq_t two_k_minus1 = (seq_t)k * 2 - 1;
    seq_t tmp = km1 * kk * two_k_minus1;
    seq_t base = (seq_t)1 + ((seq_t)2 * tmp) / (seq_t)3;
    seq_t two_k = (seq_t)k * 2;
    return base + (seq_t)t * two_k + km1;
}

seq_t a(idx_t n)
{
    seq_t r = A167381_a(n / 2);
    if (n & 1)
        r += 1;
    return r;
}
