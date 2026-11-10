/*
 * A271720  a(1)=1; for n>1, define a sequence {b(m), m >= 1} by b(1)=n
 *          b(2)=13, and b(m) = A020639(b(m-2)) + A006530(b(m-1)); then a(n)
 *          is the number of terms in that sequence before the first of the
 *          infinite string of 4s.
 * https://oeis.org/A271720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   b(1)=n, b(2)=13, b(m) = A020639(b(m-2)) + A006530(b(m-1)); a(n)
 *            = number of terms before the first of the trailing run of 4s
 *            (simulation)
 * Range:     u8 n <= 1354, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 94 terms through n = 9539 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A271720_lpf(seq_t n)
{
    if (n <= 1) return 1;
    if (n % 2 == 0) return 2;
    for (seq_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;
}

static seq_t A271720_gpf(seq_t n)
{
    if (n <= 1) return 1;
    seq_t g = 1;
    while (n % 2 == 0) { g = 2; n /= 2; }
    for (seq_t i = 3; i <= n / i; i += 2) {
        while (n % i == 0) { g = i; n /= i; }
    }
    if (n > 1) g = n;
    return g;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n, y = 13;
    seq_t m = 1;
    for (;;) {
        if (x == 4 && y == 4) return m - 1;
        seq_t z = A271720_lpf(x) + A271720_gpf(y);
        x = y;
        y = z;
        m++;
    }
}
