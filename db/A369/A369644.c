/*
 * A369644  Positions of multiples of 3 in A083345.
 * https://oeis.org/A369644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th multiple of 3 in A083345, where
 *            A083345(k) = k'/gcd(k,k') and k' is the arithmetic derivative of
 *            k. Found by a scan over k with trial-division factoring, cached
 *            in a table
 * Range:     u8 n <= 51, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A369644_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A369644_num(uint64_t n) {
    if (n == 1) return 0;
    uint64_t temp = n, d = 0;
    for (uint64_t p = 2; temp > 1; ++p) {
        if (p * p > temp) p = temp;
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            d += e * (n / p);
        }
    }
    uint64_t g = A369644_gcd(n, d);
    return d / g;
}

static uint64_t *A369644_found = 0;
static uint64_t A369644_cnt = 0, A369644_cap = 0, A369644_next = 1;

seq_t a(idx_t n) {
    while (A369644_cnt < (uint64_t)n) {
        uint64_t k = A369644_next++;
        if (A369644_num(k) % 3 == 0) {
            if (A369644_cnt == A369644_cap) {
                A369644_cap = A369644_cap ? A369644_cap * 2 : 256;
                A369644_found = (uint64_t *)realloc(A369644_found, A369644_cap * sizeof(uint64_t));
            }
            A369644_found[A369644_cnt++] = k;
        }
    }
    return (seq_t)A369644_found[n - 1];
}
