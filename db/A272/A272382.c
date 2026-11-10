/*
 * A272382  Primes p == 1 (mod 3) for which A261029(14*p) = 3.
 * https://oeis.org/A272382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th prime p == 1 (mod 3) with A261029(14*p) = 3; found
 *            by search over p < 196 (proved bound 4*7^2), counting
 *            x^3+y^3+z^3-3xyz = 14p directly
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static long long A261029_a(long long n)
{
    long long cnt = 0;
    long long max_xy = (n + 2) / 3;
    for (long long x = 0; x <= max_xy; ++x) {
        long long x3 = x * x * x;
        bool any_x = false;
        for (long long y = x; y <= max_xy; ++y) {
            long long y3 = y * y * y;
            long long z = (y > x + 1) ? y : (x + 1);
            long long f = x3 + y3 + z * z * z - 3 * x * y * z;
            if (f > n) break;
            any_x = true;
            while (f <= n) {
                if (f == n) cnt++;
                long long inc = 3 * z * z + 3 * z + 1 - 3 * x * y;
                ++z;
                f += inc;
            }
        }
        (void)any_x;
    }
    return cnt;
}

static bool A272382_isprime(long long p)
{
    if (p < 2) return false;
    for (long long d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static int built = 0;
    static long long terms[64];
    static int nterms = 0;
    if (!built) {
        for (long long p = 7; p < 200; p += 6) { /* p == 1 mod 3 */
            if (!A272382_isprime(p)) continue;
            if (A261029_a(14 * p) == 3 && nterms < 64)
                terms[nterms++] = p;
        }
        built = 1;
    }
    if (n < 1 || n > (idx_t)nterms) return (seq_t)0;
    return (seq_t)terms[(int)(n - 1)];
}
