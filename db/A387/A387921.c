/*
 * A387921  a(n) is the least number k such that A005704(k) is divisible by
 *          prime(n).
 * https://oeis.org/A387921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 with prime(n) | A005704(k), where A005704(k)
 *            = A005704(k-1) + A005704(floor(k/3)), A005704(0) = 1; found by
 *            search modulo prime(n)
 * Range:     u8 n <= 22, u16 n <= 1529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9892 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n) {
    uint64_t p = nth_prime(n);
    size_t cap = 1024, len = 1;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    v[0] = 1 % p;
    for (;;) {
        size_t k = len;
        if (k >= cap) {
            cap *= 2;
            v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
        }
        uint64_t x = v[k - 1] + v[k / 3];
        if (x >= p) x -= p;
        v[k] = x;
        len++;
        if (x == 0) {
            free(v);
            return (seq_t)k;
        }
    }
}
