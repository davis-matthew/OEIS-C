/*
 * A391439  2-rank of the class group of the order of real quadratic fields
 *          with discriminant A079896(n).
 * https://oeis.org/A391439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with D = A079896(n), a(n) = omega(|D|) - t - [16|D or D has a
 *            prime factor == 3 (mod 4)], where t = 0 if 32|D, t = 2 if D == 4
 *            (mod 16), t = 1 otherwise. D is found by search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 110 terms through n = 9680 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t found = 0, d = 1, r = 1;
    while (found < (uint64_t)n) {
        d++;
        while (r * r < d) r++;
        if ((d & 3) < 2 && r * r != d)
            found++;
    }
    uint64_t D = d, m = D;
    int omega = 0;
    bool has3 = false;
    if ((m & 1) == 0) {
        omega++;
        while ((m & 1) == 0) m >>= 1;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            omega++;
            if ((p & 3) == 3) has3 = true;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        omega++;
        if ((m & 3) == 3) has3 = true;
    }
    int t;
    if ((D & 31) == 0) t = 0;
    else if ((D & 15) == 4) t = 2;
    else t = 1;
    int res = omega - t;
    if ((D & 15) == 0 || has3) res--;
    if (res < 0) res = 0;
    return (seq_t)res;
}
