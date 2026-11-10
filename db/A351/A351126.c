/*
 * A351126  a(n) = A195199(n) / n.
 * https://oeis.org/A351126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k, the least k>=1 with tau(k*n) > 2*tau(n)
 *            (A195199(n)/n); search over k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9994 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t pr[24];
    uint64_t ex[24];
    int cnt = 0;
    uint64_t x = (uint64_t)n;
    for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            pr[cnt] = p; ex[cnt] = e; cnt++;
        }
    }
    if (x > 1) { pr[cnt] = x; ex[cnt] = 1; cnt++; }

    uint64_t tn = 1;
    for (int i = 0; i < cnt; i++) tn *= (ex[i] + 1);

    for (uint64_t k = 1; ; k++) {
        uint64_t add[24];
        for (int i = 0; i < cnt; i++) add[i] = 0;
        uint64_t extra = 1;
        uint64_t y = k;
        for (uint64_t p = 2; p * p <= y; p += (p == 2 ? 1 : 2)) {
            if (y % p == 0) {
                uint64_t e = 0;
                while (y % p == 0) { y /= p; e++; }
                bool found = false;
                for (int i = 0; i < cnt; i++)
                    if (pr[i] == p) { add[i] += e; found = true; break; }
                if (!found) extra *= (e + 1);
            }
        }
        if (y > 1) {
            bool found = false;
            for (int i = 0; i < cnt; i++)
                if (pr[i] == y) { add[i] += 1; found = true; break; }
            if (!found) extra *= 2;
        }
        uint64_t t = extra;
        for (int i = 0; i < cnt; i++) t *= (ex[i] + add[i] + 1);
        if (t > 2 * tn) return (seq_t)k;
    }
}
