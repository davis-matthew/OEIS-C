/*
 * A359488  Run lengths of A359487.
 * https://oeis.org/A359488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal values in A359487
 *            (nondecreasing); A359487(m) = least k>=m+1 with k and k+1 both
 *            having a prime factor > m. Computed by search: scan m upward,
 *            grouping equal consecutive values
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A359488_hpf(uint64_t x, uint64_t n) {
    if (x <= 1) return false;
    for (uint64_t d = 2; d * d <= x; ++d) {
        if (x % d == 0) {
            if (d > n) return true;
            while (x % d == 0) x /= d;
        }
    }
    return x > n;
}

static uint64_t A359488_av(uint64_t m, uint64_t lo) {
    uint64_t k = m + 1;
    if (lo > k) k = lo;
    while (!(A359488_hpf(k, m) && A359488_hpf(k + 1, m))) ++k;
    return k;
}

seq_t a(idx_t n) {
    uint64_t target = (uint64_t)n;
    uint64_t run = 1;
    uint64_t cur = A359488_av(1, 0);
    uint64_t cnt = 1;
    for (uint64_t m = 2;; ++m) {
        uint64_t v = A359488_av(m, cur);
        if (v == cur) {
            ++cnt;
        } else {
            if (run == target) return (seq_t)cnt;
            ++run;
            cur = v;
            cnt = 1;
        }
    }
}
