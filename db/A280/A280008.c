/*
 * A280008  First differences of A002375.
 * https://oeis.org/A280008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A002375(n+1) - A002375(n); A002375(m) counted by sieving
 *            primes up to 2m and counting odd prime pairs p+q=2m
 * Range:     i8 n <= 2308, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 8992 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static long long A280008_goldbach(const bool *comp, uint64_t m) {
    long long count = 0;
    uint64_t target = m * 2, p;
    if (m < 3) return 0;
    for (p = 3; p <= target / 2; p += 2) {
        if (!comp[p] && !comp[target - p]) count++;
    }
    return count;
}

seq_t a(idx_t n) {
    uint64_t top, i, j;
    bool *comp;
    long long d;

    top = (uint64_t)n * 2 + 2;
    comp = (bool *)calloc((size_t)(top + 1), sizeof(bool));
    if (!comp) return 0;
    comp[0] = comp[1] = true;
    for (i = 2; i * i <= top; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= top; j += i) comp[j] = true;
        }
    }
    d = A280008_goldbach(comp, (uint64_t)n + 1) - A280008_goldbach(comp, (uint64_t)n);
    free(comp);
    return (seq_t)d;
}
