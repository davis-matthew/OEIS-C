/*
 * A340086  a(1) = 0, for n > 1, a(n) = A000265(n-1) / gcd(n-1, A336466(n)).
 * https://oeis.org/A340086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = A000265(n-1) / gcd(n-1, A336466(n)),
 *            where A336466 is fully multiplicative with A336466(p) =
 *            A000265(p-1)
 * Range:     u8 n <= 256, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 106 terms through n = 8040 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A340086_odd(seq_t x)
{
    if (x == 0) return 0;
    while ((x & 1) == 0) x >>= 1;
    return x;
}

static seq_t A340086_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static seq_t A340086_mult(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t o = A340086_odd((seq_t)p - 1);
            while (m % p == 0) {
                m /= p;
                result *= o;
            }
        }
    }
    if (m > 1)
        result *= A340086_odd((seq_t)m - 1);
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    seq_t nm1 = (seq_t)(n - 1);
    seq_t g = A340086_gcd(nm1, A340086_mult(n));
    return A340086_odd(nm1) / g;
}
