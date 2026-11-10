/*
 * A381501  Positive integers that do not appear in A381466.
 * https://oeis.org/A381501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A381466, where A381466(0)=4,
 *            A381466(n)=A381466(n-1)+n if gcd(A381466(n-1),n)=1, else
 *            n/gcd(A381466(n-1),n); computed by running A381466 for many
 *            terms and sieving the values seen
 * Range:     u8 n <= 50, u16 n <= 366, u32 n <= 366, u64 n <= 366,
 *            u128 n <= 366, big any n
 * Validated: standard, pass, 87 terms through n = 358 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t A381501_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t L = 8 * (uint64_t)n + 100;
    for (;;) {
        unsigned char *seen = (unsigned char *)calloc((size_t)L + 1, 1);
        if (!seen) abort();
        uint64_t M = 64 * L;
        uint64_t prev = 4;
        seen[4] = 1;
        for (uint64_t i = 1; i <= M; i++) {
            uint64_t g = A381501_gcd(prev, i);
            uint64_t v = (g == 1) ? prev + i : i / g;
            if (v <= L) seen[v] = 1;
            prev = v;
        }
        uint64_t cnt = 0;
        for (uint64_t v = 1; v <= L; v++) {
            if (!seen[v]) {
                if (++cnt == (uint64_t)n) {
                    free(seen);
                    return (seq_t)v;
                }
            }
        }
        free(seen);
        L *= 2;
    }
}
