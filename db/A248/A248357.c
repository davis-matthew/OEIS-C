/*
 * A248357  Numbers k such that A248355(k+1) = A248355(k) + 1.
 * https://oeis.org/A248357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A248355(k+1) = A248355(k) + 1; computed
 *            by a linear scan over k with A248355 tracked incrementally (it
 *            is nondecreasing)
 * Range:     u8 n <= 48, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Validated: standard, pass, 58 terms through n = 287 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static int A248357_ok(idx_t k, idx_t m)
{
    static const long double PI =
        3.141592653589793238462643383279502884L;
    const long double kd = (long double)k;
    const long double val = PI - kd * sinl(PI / kd);
    const long double thresh = 1.0L / (2.0L * (long double)m);
    return val < thresh;
}

seq_t a(idx_t n)
{
    idx_t k = 1;
    idx_t m = 1;
    idx_t count = 0;

    while (!A248357_ok(k, m)) k++;
    idx_t cur = k;

    for (;; m++) {
        idx_t mm = m + 1;
        while (!A248357_ok(k, mm)) k++;
        if (k == cur + 1) {
            count++;
            if (count == n) return (seq_t)(unsigned long long)m;
        }
        cur = k;
    }
}
