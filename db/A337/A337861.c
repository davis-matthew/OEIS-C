/*
 * A337861  Numbers that can be written as the sum of two Moran numbers (see
 *          A001101).
 * https://oeis.org/A337861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k that equals A001101(i)+A001101(j); computed by a
 *            search over Moran numbers (k/digitsum(k) prime) with a doubling
 *            limit and sieve of pair sums
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A337861_isprime(uint64_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static bool A337861_moran(uint64_t k) {
    uint64_t s = 0, t = k;
    while (t) { s += t % 10; t /= 10; }
    if (s == 0 || k % s) return false;
    return A337861_isprime(k / s);
}

seq_t a(idx_t n) {
    uint64_t L = 256;
    for (;;) {
        uint8_t *mark = (uint8_t *)calloc((size_t)L + 1, 1);
        uint64_t *mor = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L + 1));
        size_t m = 0;
        for (uint64_t k = 1; k <= L; k++)
            if (A337861_moran(k)) mor[m++] = k;
        for (size_t i = 0; i < m; i++)
            for (size_t j = i; j < m; j++) {
                uint64_t s = mor[i] + mor[j];
                if (s > L) break;
                mark[s] = 1;
            }
        idx_t cnt = 0;
        uint64_t res = 0;
        for (uint64_t k = 1; k <= L; k++) {
            if (mark[k]) {
                cnt++;
                if (cnt == n) { res = k; break; }
            }
        }
        free(mark);
        free(mor);
        if (res) return (seq_t)res;
        L *= 2;
    }
}
