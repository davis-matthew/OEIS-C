/*
 * A319661  2-rank of the class group of imaginary quadratic field with
 *          discriminant -k, k = A191483(n).
 * https://oeis.org/A319661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(A191483(n)) - 1, with A191483(n) = 4m, m the n-th
 *            squarefree number == 1 or 2 (mod 4); found by linear search
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A319661_sqfree(uint64_t m)
{
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return false;
        }
    }
    return true;
}

static int A319661_omega(uint64_t m)
{
    int c = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            c++;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) c++;
    return c;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t m = 1; ; m++) {
        uint64_t r = m & 3;
        if ((r == 1 || r == 2) && A319661_sqfree(m)) {
            count++;
            if (count == n) {
                int w = A319661_omega(m);
                /* m odd: omega(4m)-1 = omega(m); m even: omega(4m)-1 = omega(m)-1 */
                if (r == 2) w -= 1;
                return (seq_t)w;
            }
        }
    }
}
