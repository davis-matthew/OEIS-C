/*
 * A344686  Triangle T(n, k) obtained from the array N2(a, b) = a^2 - a*b -
 *          b^2, for a >= 0 and b >= 0, read by upwards antidiagonals.
 * https://oeis.org/A344686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(d,k) where * d = floor((sqrt(8*n+1)-1)/2), * k = n -
 *            d*(d+1)/2, * T(d,k) = d^2 - 3*d*k + k^2
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;           // 2^32 > sqrt(2^64-1)
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Diagonal index */
    uint64_t d = (isqrt_u64(8ULL * n + 1ULL) - 1ULL) >> 1;
    /* Number of terms before this diagonal */
    uint64_t prev = (unsigned __int128)d * (d + 1ULL) / 2ULL;
    /* Offset within the diagonal (upwards order) */
    uint64_t k = n - (idx_t)prev;

    seq_t dseq = (seq_t)d;
    seq_t kseq = (seq_t)k;
    seq_t val  = dseq * dseq - (seq_t)3 * dseq * kseq + kseq * kseq;

    return val;
}
