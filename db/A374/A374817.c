/*
 * A374817  Irregular table T(n, k), n > 0, k = 1..A000005(n), read by rows;
 *          the n-th row lists the numbers m such that A003991(m) = n.
 * https://oeis.org/A374817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n lists the positions m = (i+j-2)*(i+j-1)/2 + i over all
 *            factorizations n = i*j (the antidiagonal positions where
 *            A003991(m) = n), sorted ascending; a(idx) is found by walking
 *            row lengths A000005
 * Range:     u8 n <= 75, u16 n <= 2192, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Validated: standard, pass, 82 terms through n = 6439 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t T374817_tau(uint64_t n)
{
    uint64_t r = 1, p;
    for (p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) { n /= p; e++; }
            r *= (e + 1);
        }
    }
    if (n > 1) r *= 2;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;
    uint64_t row = 1, d, cnt = 0, i;
    uint64_t *v;
    uint64_t res;

    if (idx == 0) return (seq_t)0;
    for (;;) {
        d = T374817_tau(row);
        if (idx <= d) break;
        idx -= d;
        row++;
    }
    v = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)d);
    for (i = 1; i <= row; i++) {
        if (row % i == 0) {
            uint64_t j = row / i;
            uint64_t s = i + j;
            uint64_t m = (s - 2) * (s - 1) / 2 + i;
            uint64_t p = cnt;
            while (p > 0 && v[p - 1] > m) {
                v[p] = v[p - 1];
                p--;
            }
            v[p] = m;
            cnt++;
        }
    }
    res = v[idx - 1];
    free(v);
    return (seq_t)res;
}
