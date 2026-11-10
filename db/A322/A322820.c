/*
 * A322820  a(n) = A052126(n) * A006530(A052126(n)).
 * https://oeis.org/A322820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A052126(n) * A006530(A052126(n)); A052126(n) = n /
 *            gpf(n), gpf(1)=1
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9936 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A006530_gpf(idx_t m)
{
    idx_t g = 1;
    while (m % 2 == 0) { g = 2; m /= 2; }
    for (idx_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) { g = i; m /= i; }
    }
    if (m > 1) g = m;
    return g;
}

seq_t a(idx_t n)
{
    idx_t m = n / A006530_gpf(n);   /* A052126(n) */
    idx_t r = m * A006530_gpf(m);   /* <= n, no overflow */
    return (seq_t)r;
}
