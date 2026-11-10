/*
 * A363482  Denominator of the continued fraction
 *          1/(2-3/(3-4/(4-5/(...(n-1)-n/(-5))))).
 * https://oeis.org/A363482
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+3n-5)/gcd(n^2+3n-5, 5*A051403(n-3)+n*A051403(n-4)),
 *            where A051403(k)=(k+2)*(A051403(k-1)-A051403(k-2)),
 *            A051403(-1)=0, A051403(0)=1; A051403 is computed mod N =
 *            n^2+3n-5
 * Range:     u8 n <= 17, u16 n <= 258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9747 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A363482_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n * (uint64_t)n + 3ULL * (uint64_t)n - 5ULL;
    uint64_t prev = 0 % N;      /* A051403(-1) */
    uint64_t cur = 1 % N;       /* A051403(0)  */
    for (uint64_t k = 1; k + 3 <= (uint64_t)n; ++k) {
        uint64_t d = (cur + N - prev) % N;
        uint64_t nx = (uint64_t)(((unsigned __int128)d * (k + 2)) % N);
        prev = cur;
        cur = nx;
    }
    /* now cur = A051403(n-3), prev = A051403(n-4), both mod N */
    unsigned __int128 m = (unsigned __int128)5 * cur + (unsigned __int128)((uint64_t)n % N) * prev;
    uint64_t mm = (uint64_t)(m % N);
    uint64_t g = A363482_gcd(N, mm);
    return (seq_t)(N / g);
}
