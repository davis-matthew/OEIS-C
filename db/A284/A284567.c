/*
 * A284567  a(0) = 1; for n >= 1, a(n) = A059897(n, a(n-1)).
 * https://oeis.org/A284567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = A059897(n, a(n-1)) for n >= 1, where
 *            A059897(x,y) = prod p^(v_p(x) XOR v_p(y)). Computed by tracking
 *            the exponent vector of a(n): exponent of each prime p is XORed
 *            with v_p(m) for m = 1..n
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 249 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    unsigned int *ex = (unsigned int *)calloc((size_t)n + 2, sizeof(unsigned int));
    if (!ex) return 0;
    for (idx_t m = 2; m <= n; m++) {
        idx_t r = m;
        for (idx_t p = 2; p <= r / p; p++) {
            if (r % p == 0) {
                unsigned int e = 0;
                while (r % p == 0) { r /= p; e++; }
                ex[p] ^= e;
            }
        }
        if (r > 1) ex[r] ^= 1u;
    }
    seq_t res = 1;
    for (idx_t p = 2; p <= n; p++) {
        for (unsigned int e = ex[p]; e > 0; e--) res *= (seq_t)p;
    }
    free(ex);
    return res;
}
