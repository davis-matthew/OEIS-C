/*
 * A323879  Number of divisors d of n such that A276154(d) divides n.
 * https://oeis.org/A323879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [A276154(d) | n], A276154 = shift of primorial
 *            base representation left one digit
 * Range:     u8 n <= 30030, u16 n <= 30030, u32 n <= 30030, u64 n <= 30030,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 121 terms through n = 9934 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned _A323879_P[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};

static unsigned __int128 _A323879_f(uint64_t d)
{
    unsigned __int128 res = 0, P = 1;
    uint64_t m = d;
    int i = 0;
    while (m > 0 && i < 20) {
        uint64_t p = _A323879_P[i];
        uint64_t dig = m % p;
        res += (unsigned __int128)dig * P * p;
        m /= p;
        P *= p;
        i++;
    }
    return res;
}

static int _A323879_check(uint64_t d, uint64_t n)
{
    unsigned __int128 f = _A323879_f(d);
    if (f == 0 || f > (unsigned __int128)n) return 0;
    return (n % (uint64_t)f) == 0;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;
    for (uint64_t d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            cnt += _A323879_check(d, N);
            uint64_t e = N / d;
            if (e != d) cnt += _A323879_check(e, N);
        }
    }
    return (seq_t)cnt;
}
