/*
 * A335438  Number of partitions of k_n into two distinct parts (s,t) such
 *          that k_n | s*t, where k_n = A335437(n).
 * https://oeis.org/A335438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((A000188(k)-1)/2) with k = A335437(n); k is found
 *            by search (k is a term iff 2*A019554(k) < k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 108 terms through n = 9978 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A335438_test(uint64_t k, uint64_t *sq) {
    uint64_t m = k, l = 1, s = 1, p;
    for (p = 2; p <= m / p; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            unsigned e = 0, i;
            while (m % p == 0) { m /= p; e++; }
            for (i = 0; i < (e + 1) / 2; i++) l *= p;
            for (i = 0; i < e / 2; i++) s *= p;
        }
    }
    if (m > 1) l *= m;
    *sq = s;
    return 2 * l < k;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    idx_t found = 0;
    uint64_t k, s = 1;
    for (k = 9; ; ++k) {
        if (A335438_test(k, &s)) {
            ++found;
            if (found == n) return (seq_t)((s - 1) / 2);
        }
    }
}
