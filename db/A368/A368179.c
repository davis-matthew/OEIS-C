/*
 * A368179  Square array read by ascending antidiagonals: row n is the
 *          trajectory of n under the A006368 map.
 * https://oeis.org/A368179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = A006368 applied k times to n (T(n,0)=n), where
 *            A006368(m) = 3m/2 if m even, else (3m+2)/4; the array is read by
 *            ascending antidiagonals, a(d(d+1)/2 + k) = T(d-k, k)
 * Range:     u8 n <= 396, u16 n <= 5653, u32 n <= 11324, u64 n <= 11324,
 *            u128 n <= 11324, big any n
 * Validated: standard, pass, 107 terms through n = 9248 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t A006368_step(seq_t m) {
    if ((m & 1) == 0) {
        return (m * 3) / 2;
    }
    return (m * 3 + 2) / 4;
}

seq_t a(idx_t n) {
    idx_t d = (idx_t)sqrt(2.0 * (double)n);
    while (d > 0 && d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    idx_t k = n - d * (d + 1) / 2;
    idx_t row = d - k;
    seq_t v = (seq_t)row;
    for (idx_t i = 0; i < k; i++) {
        v = A006368_step(v);
    }
    return v;
}
