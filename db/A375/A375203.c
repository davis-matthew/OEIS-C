/*
 * A375203  Positions of records in A375202.
 * https://oeis.org/A375203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n-th record in A375202 (A375202(0)=0 first
 *            record); computed by brute-force search for the first terms,
 *            then using a(n) = 4*a(n-4) for n >= 6
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 52 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A375203_two_sq(uint64_t m)
{
    if (m == 0) return true;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if ((p & 3) == 3 && (e & 1)) return false;
        }
    }
    if (t > 1 && (t & 3) == 3) return false;
    return true;
}

static long long A375203_f(long long k)
{
    for (long long x = 0; x * x <= k; ++x)
        if (A375203_two_sq((uint64_t)(k - x * x))) return x;
    return -1;
}

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t mult = 1;
    while (m > 9) { m -= 4; mult *= 4; }
    long long best = -1;
    idx_t cnt = 0;
    for (long long k = 0; ; ++k) {
        long long v = A375203_f(k);
        if (v > best) {
            best = v;
            ++cnt;
            if (cnt == m) return (seq_t)k * mult;
        }
    }
}
