/*
 * A325815  a(n) = A324213(n) - A034444(n).
 * https://oeis.org/A325815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A324213(n) - A034444(n), where A324213(n) =
 *            #{0<=k<=sigma(n): gcd(n-k, 2n-sigma(n)) = 1} (computed as
 *            gcd(|n-i|, |sigma(n)-i-n|) over i) and A034444(n) = 2^omega(n)
 * Range:     i8 n <= 59, i16 n <= 14399, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 105 terms through n = 9945 (2026-11-08)
 * Helpers:   gcd, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/omega.h"

static seq_t A324213_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t A324213_a(idx_t n)
{
    seq_t sigma = A324213_sigma_of(n);
    idx_t sigma_idx = (idx_t)sigma;
    seq_t count = 0;

    for (idx_t i = 0; i <= sigma_idx; ++i) {
        seq_t a_abs = (i > n) ? (seq_t)(i - n) : (seq_t)(n - i);
        seq_t diff = sigma - (seq_t)i;
        seq_t b_abs = (diff > (seq_t)n) ? diff - (seq_t)n : (seq_t)n - diff;
        if (gcd(a_abs, b_abs) == 1)
            ++count;
    }
    return count;
}

static seq_t A034444_a(idx_t n)
{
    if (n == 0) return 0;
    size_t distinct = (size_t)omega((uint64_t)n);
    seq_t result = 1;
    for (size_t i = 0; i < distinct; ++i)
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    return A324213_a(n) - A034444_a(n);
}
