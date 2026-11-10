/*
 * A335875  a(n) = min(A329697(n), A331410(n)).
 * https://oeis.org/A335875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(A329697(n), A331410(n)), where A329697 counts steps
 *            of k -> k - k/lpf(k) and A331410 counts steps of k -> k +
 *            k/lpf(k), each until a power of 2 is reached (lpf = largest
 *            prime factor)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8908 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A335875_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    uint64_t k;
    uint64_t c1 = 0, c2 = 0;

    if (n <= 1)
        return 0;

    k = (uint64_t)n;
    while ((k & (k - 1)) != 0) {
        k -= k / A335875_lpf(k);
        ++c1;
    }

    k = (uint64_t)n;
    while ((k & (k - 1)) != 0 && c2 < c1) {
        k += k / A335875_lpf(k);
        ++c2;
    }
    /* if the second loop was cut off at c1, it has c2 >= c1, so min is c1 */
    return (seq_t)(c1 < c2 ? c1 : c2);
}
