/*
 * A334204  a(n) = A329697(A163511(n)).
 * https://oeis.org/A334204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329697(A163511(n)), computed on the prime-exponent
 *            vector of A163511(n): repeatedly replace one copy of the largest
 *            odd prime p by the factorization of p-1, i.e. k -> k - k/p, and
 *            count steps until only factors of 2 remain. a(0) = 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9816 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t pr[64];
    uint32_t e[64];
    int i, j, np = 0;
    uint64_t c;
    for (c = 2; np < 64; c++) {
        bool ok = true;
        for (j = 0; j < np && pr[j] * pr[j] <= c; j++)
            if (c % pr[j] == 0) { ok = false; break; }
        if (ok) pr[np++] = c;
    }
    for (i = 0; i < 64; i++) e[i] = 0;
    if (n == 0) return (seq_t)0;

    int r = 0;
    int prev = 0, bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            e[r] += (uint32_t)(bitpos - prev - 1);
            prev = bitpos;
            r++;
        }
        bitpos++;
        tmp >>= 1;
    }
    e[r - 1] += 1;   /* extra factor p(r) */

    seq_t count = 0;
    for (;;) {
        int top = -1;
        for (i = 63; i >= 0; i--)
            if (e[i]) { top = i; break; }
        if (top <= 0) break;
        e[top]--;
        uint64_t m = pr[top] - 1;
        for (j = 0; j < top && m > 1; j++) {
            while (m % pr[j] == 0) { m /= pr[j]; e[j]++; }
        }
        count++;
    }
    return count;
}
