/*
 * A383425  Indices k such that A003849(k) = 0 and A383422(k) = 1.
 * https://oeis.org/A383425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 0 with A003849(k) = 0 and A383422(k) = 1;
 *            computed by a linear scan over k, evaluating both words by block
 *            decomposition
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* word with s(0)="0", s(1)=0 followed by (l1-1) ones, s(k)=s(k-1)s(k-2) */
static int A383425_word(uint64_t n, uint64_t l1) {
    uint64_t len[100];
    unsigned k = 1;
    len[0] = 1;
    len[1] = l1;
    while (len[k] <= n) {
        ++k;
        len[k] = len[k-1] + len[k-2];
    }
    while (k > 1) {
        if (n < len[k-1]) {
            --k;
        } else {
            n -= len[k-1];
            k -= 2;
        }
    }
    if (k == 0) return 0;
    return (n == 0) ? 0 : 1;
}

seq_t a(idx_t n) {
    uint64_t cnt = 0;
    uint64_t k = 0;
    for (;; ++k) {
        if (A383425_word(k, 2) == 0 && A383425_word(k, 3) == 1) {
            ++cnt;
            if (cnt == (uint64_t)n) return (seq_t)k;
        }
    }
}
