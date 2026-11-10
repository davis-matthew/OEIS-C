/*
 * A325519  Difference sequence of A325426.
 * https://oeis.org/A325519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325426(n+1) - A325426(n); A325426 found by a sieve (2*x
 *            and floor(3*x/2) of non-members x>1 are members)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9102 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t limit = (size_t)n * 8 + 64;
    for (;;) {
        size_t cap = 2 * limit + 4;
        unsigned char *v = (unsigned char *)calloc(cap, 1);
        if (!v) return 0;
        idx_t count = 0;
        size_t prev = 0;
        int found = 0;
        size_t res = 0;
        for (size_t i = 1; i <= limit; ++i) {
            if (v[i]) {
                ++count;
                if (count == n) prev = i;
                else if (count == n + 1) {
                    res = i - prev;
                    found = 1;
                    break;
                }
            } else {
                v[2 * i] = 1;
                if (i > 1) v[(i * 3) / 2] = 1;
            }
        }
        free(v);
        if (found) return (seq_t)res;
        limit *= 2;
    }
}
