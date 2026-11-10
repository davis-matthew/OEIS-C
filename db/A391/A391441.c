/*
 * A391441  2-rank of the form class group of the order of real quadratic
 *          fields with discriminant A079896(n).
 * https://oeis.org/A391441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(|D|) - t with D = A079896(n); t = 0 if 32|D, t = 2
 *            if D == 4 (mod 16), else t = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9780 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t d = 1, r = 1;
    uint64_t found = 0;
    uint64_t D = 0;
    while (found < (uint64_t)n) {
        d++;
        while ((r + 1) * (r + 1) <= d) r++;
        if ((d & 3) <= 1 && r * r != d) {
            found++;
            D = d;
        }
    }
    uint64_t m = D;
    int w = 0;
    if ((m & 1) == 0) {
        w++;
        while ((m & 1) == 0) m >>= 1;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            w++;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) w++;
    int t;
    if (D % 32 == 0) t = 0;
    else if (D % 16 == 4) t = 2;
    else t = 1;
    return (seq_t)(w - t);
}
