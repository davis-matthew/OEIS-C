/*
 * A329267  Duplicate of A100771.
 * https://oeis.org/A329267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = min(|n - p|) over palindromes p, i.e. distance from n to
 *            the nearest palindrome (same as A100771)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-08)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

static seq_t A329267_p10(int e)
{
    seq_t r = 1;
    for (int i = 0; i < e; ++i) r *= 10;
    return r;
}

static seq_t A329267_make_pal(seq_t leftHalf, int leftHalfLen, int totalLen)
{
    int leftDigits[24];
    seq_t tmp = leftHalf;
    for (int i = leftHalfLen - 1; i >= 0; --i) {
        leftDigits[i] = (int)(tmp % 10);
        tmp /= 10;
    }
    seq_t pal = 0;
    for (int i = 0; i < totalLen; ++i) {
        int d = (i < leftHalfLen) ? leftDigits[i] : leftDigits[totalLen - 1 - i];
        pal = pal * 10 + d;
    }
    return pal;
}

seq_t a(idx_t n)
{
    if (n < 10) return 0;

    int len = num_digits_idx(n);
    int half = (len + 1) / 2;
    seq_t N = (seq_t)n;

    seq_t leftHalf = N / A329267_p10(len - half);
    seq_t p1 = A329267_make_pal(leftHalf, half, len);
    if (p1 == N) return 0;

    seq_t lo, hi;
    if (p1 > N) {
        hi = p1;
        seq_t dec = leftHalf - 1;
        if (dec < A329267_p10(half - 1))
            lo = A329267_p10(len - 1) - 1;
        else
            lo = A329267_make_pal(dec, half, len);
    } else {
        lo = p1;
        seq_t inc = leftHalf + 1;
        if (inc > A329267_p10(half) - 1)
            hi = A329267_p10(len) + 1;
        else
            hi = A329267_make_pal(inc, half, len);
    }

    seq_t dl = N - lo;
    seq_t dr = hi - N;
    return (dl < dr) ? dl : dr;
}
