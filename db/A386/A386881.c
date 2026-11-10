/*
 * A386881  a(n) is the number of occurrences of n in A386838.
 * https://oeis.org/A386881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m : min over x^2+y^2=m, x,y>=0 of (x+y-gcd(x,y)) = n
 *            }; computed by sieve over all (x,y) with x^2+y^2 <= 2n^2
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t a386881_gcd(uint64_t p, uint64_t q) {
    while (q) { uint64_t t = p % q; p = q; q = t; }
    return p;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t lim = 2 * N * N;
    uint64_t *best = (uint64_t *)malloc((lim + 1) * sizeof(uint64_t));
    if (!best) abort();
    for (uint64_t i = 0; i <= lim; i++) best[i] = UINT64_MAX;
    for (uint64_t x = 0; x * x <= lim; x++) {
        for (uint64_t y = 0; y <= x; y++) {
            uint64_t m = x * x + y * y;
            if (m > lim) break;
            uint64_t f = x + y - a386881_gcd(x, y);
            if (f < best[m]) best[m] = f;
        }
    }
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= lim; i++)
        if (best[i] == N) cnt++;
    free(best);
    return (seq_t)cnt;
}
