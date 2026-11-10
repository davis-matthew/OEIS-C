/*
 * A273191  a(n) is the length of the n-th run of A273190.
 * https://oeis.org/A273191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = length of the n-th run of equal consecutive terms of
 *            A273190, where A273190(m) = floor(sqrt(2*m-1)) -
 *            floor(sqrt(m-1)) for m>0 and A273190(0) = 0. Computed by a
 *            linear scan with incrementally tracked integer square roots
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9261 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t runs = 0;
    uint64_t len = 1;       /* run containing index 0 (value 0) */
    uint64_t prev = 0;
    uint64_t s1 = 1, s2 = 0;
    uint64_t i;

    for (i = 1;; i++) {
        uint64_t x1 = 2 * i - 1;
        uint64_t x2 = i - 1;
        uint64_t v;
        while ((s1 + 1) * (s1 + 1) <= x1) s1++;
        while ((s2 + 1) * (s2 + 1) <= x2) s2++;
        v = s1 - s2;
        if (v == prev) {
            len++;
        } else {
            runs++;
            if (runs == (uint64_t)n) return (seq_t)len;
            prev = v;
            len = 1;
        }
    }
}
