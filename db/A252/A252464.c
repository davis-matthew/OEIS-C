/*
 * A252464  a(1) = 0, a(2n) = 1 + a(n), a(2n+1) = 1 + a(A064989(2n+1)); also
 *          binary width of terms of A156552 and A243071.
 * https://oeis.org/A252464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = pi(lpf(n)) + bigomega(n) - 1, where
 *            lpf is the largest prime factor (this is the bit length of
 *            A156552(n))
 * Range:     u8 n <= 1618, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 119 terms through n = 7378 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A252464_pi(uint64_t p)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)p + 1, 1);
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= p; i++) {
        if (!comp[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= p && j >= i; j += i) comp[j] = 1;
        }
    }
    free(comp);
    return cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    uint64_t k = 0, lpf = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        while (m % p == 0) {
            m /= p;
            k++;
            lpf = p;
        }
    }
    if (m > 1) {
        k++;
        lpf = m;
    }
    return (seq_t)(A252464_pi(lpf) + k - 1);
}
