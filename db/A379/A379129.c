/*
 * A379129  a(n) is the number of unitary proper divisors d > 1 of n for which
 *          A048720(A065621(sigma(d)),sigma(n/d)) is equal to sigma(n).
 * https://oeis.org/A379129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1, 1<d<n}
 *            [A048720(A065621(sigma(d)),sigma(n/d)) == sigma(n)]
 * Range:     u8 n <= 65, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9876 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a048720_mul(seq_t x, seq_t y)
{
    seq_t r = 0;
    while (y != 0) {
        if ((y & 1) != 0) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

static seq_t a065621_rev(seq_t n)
{
    seq_t low = n ^ (n & (n - 1));
    return n ^ ((n - low) << 1);
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t sg[64];
    int k = 0;
    uint64_t p;
    for (p = 2; p * p <= m || (m > 1 && p == 2); p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            seq_t s = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                s += pw;
            }
            sg[k++] = s;
        }
        if (p * p > m) break;
    }
    if (m > 1) {
        seq_t s = (seq_t)m + 1;
        sg[k++] = s;
    }
    if (k < 2) return 0;
    seq_t total = 1;
    for (int i = 0; i < k; i++) total *= sg[i];
    seq_t count = 0;
    unsigned long full = (1UL << k) - 1;
    for (unsigned long mask = 1; mask < full; mask++) {
        seq_t sd = 1, sr = 1;
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1UL) sd *= sg[i];
            else sr *= sg[i];
        }
        if (a048720_mul(a065621_rev(sd), sr) == total) count++;
    }
    return count;
}
