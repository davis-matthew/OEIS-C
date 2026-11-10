/*
 * A285702  a(n) = A000010(A064216(n)).
 * https://oeis.org/A285702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A064216(n)); factor 2n-1 = prod p^e, then a(n) =
 *            prod (q-1)*q^(e-1) with q = prevprime(p)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9464 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A285702_isp(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint64_t A285702_prev(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !A285702_isp(q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    seq_t result = 1;
    uint64_t p;
    for (p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t q = A285702_prev(p);
            t /= p;
            result *= (seq_t)(q - 1);
            while (t % p == 0) {
                t /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (t > 1) {
        uint64_t q = A285702_prev(t);
        result *= (seq_t)(q - 1);
    }
    return result;
}
