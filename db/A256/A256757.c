/*
 * A256757  Number of iterations of A007733 required to reach 1.
 * https://oeis.org/A256757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; for n > 1, a(n) = a(A007733(n)) + 1, where A007733(n)
 *            is the multiplicative order of 2 modulo the odd part of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9846 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A256757_ord(idx_t n) {
    idx_t m = n;
    while (m % 2 == 0) m /= 2;
    if (m == 1) return 1;
    idx_t p = 2 % m;
    idx_t order = 1;
    while (p != 1) {
        p = (p * 2) % m;
        order++;
    }
    return order;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    while (n > 1) {
        n = A256757_ord(n);
        count++;
    }
    return count;
}
