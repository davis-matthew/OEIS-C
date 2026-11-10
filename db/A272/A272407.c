/*
 * A272407  Primes p == 1 (mod 3) for which A261029(38*p) = 3.
 * https://oeis.org/A272407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   search: a(n) = n-th prime p == 1 (mod 3) with A261029(38*p) = 3,
 *            where A261029(m) counts x<=y<=z, z>=x+1, with x^3+y^3+z^3-3xyz =
 *            m; p < 1444
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long A272407_F(long long x, long long y, long long z)
{
    return x*x*x + y*y*y + z*z*z - 3*x*y*z;
}

static int A272407_count(long long N)
{
    int cnt = 0;
    for (long long x = 0; 3*x + 1 <= N; ++x) {
        for (long long y = x; ; ++y) {
            long long z = (y > x + 1) ? y : x + 1;
            if (A272407_F(x, y, z) > N) break;
            for (;; ++z) {
                long long f = A272407_F(x, y, z);
                if (f > N) break;
                if (f == N) cnt++;
            }
        }
    }
    return cnt;
}

static int A272407_isprime(int p)
{
    if (p < 2) return 0;
    for (int d = 2; d * d <= p; ++d)
        if (p % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    static int terms[64];
    static int nterms = -1;
    if (nterms < 0) {
        nterms = 0;
        for (int p = 7; p < 1444 && nterms < 64; p += 6) {
            if (!A272407_isprime(p)) continue;
            if (A272407_count(38LL * p) == 3)
                terms[nterms++] = p;
        }
    }
    if (n < 1 || n > (idx_t)nterms) return 0;
    return (seq_t)terms[n - 1];
}
