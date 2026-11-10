/*
 * A247876  Least prime factor of A247682.
 * https://oeis.org/A247876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime factor of A247682(n), where A247682 lists the
 *            odd composite numbers == 5 (mod 9); found by scanning 18k+5 with
 *            trial division
 * Range:     u8 n <= 8, u16 n <= 3932, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9674 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A247876_lpf(seq_t m)
{
    /* m is odd and > 1 */
    seq_t d;
    for (d = 3; d * d <= m; d += 2) {
        if (m % d == 0) return d;
    }
    return m;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;
    for (;; ++k) {
        seq_t cand = (seq_t)18 * k + 5;
        seq_t p = A247876_lpf(cand);
        if (p != cand) {
            ++count;
            if (count == n) return p;
        }
    }
}
