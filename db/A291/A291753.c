/*
 * A291753  a(n) is the length of stage n in A137843.
 * https://oeis.org/A291753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + (A137843(n-1) mod 2), a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 1837 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A291753_val(uint64_t m) {
    uint64_t L[70];
    unsigned par[70];
    int s = 1;
    if (m <= 1) return 1;
    L[1] = 1;
    while (L[s] < m) {
        par[s] = (unsigned)(A291753_val((uint64_t)s) & 1u);
        L[s + 1] = 2 * L[s] + par[s];
        s++;
    }
    uint64_t pos = m;
    while (s > 1) {
        uint64_t prev = L[s - 1];
        if (par[s - 1]) {
            if (pos == prev + 1) return (uint64_t)s;
            if (pos > prev + 1) pos -= prev + 1;
        } else {
            if (pos > prev) pos -= prev;
        }
        s--;
    }
    return 1;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    for (idx_t k = 2; k <= n; k++) {
        r = r * 2 + (seq_t)(A291753_val((uint64_t)(k - 1)) & 1u);
    }
    return r;
}
