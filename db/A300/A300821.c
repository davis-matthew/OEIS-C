/*
 * A300821  Möbius transform of A244042.
 * https://oeis.org/A300821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = Sum_{d|n} moebius(n/d)*A244042(d), where A244042(n) is n
 *            in base 3 with each digit 2 replaced by 0
 * Range:     i8 n <= 223, i16 n <= 29239, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 100 terms through n = 9840 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A244042_a(idx_t n)
{
    int digits[50];
    int cnt = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[cnt++] = (int)(temp % 3);
        temp /= 3;
    }
    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        int d = digits[i];
        if (d == 2) d = 0;
        result = result * 3 + (seq_t)d;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t pr[20];
    int k = 0;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            pr[k++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) pr[k++] = m;

    seq_t total = 0;
    unsigned long mask;
    for (mask = 0; mask < (1UL << k); mask++) {
        idx_t e = 1;
        int bits = 0;
        int i;
        for (i = 0; i < k; i++) {
            if (mask & (1UL << i)) {
                e *= pr[i];
                bits++;
            }
        }
        seq_t v = A244042_a(n / e);
        if (bits & 1) total -= v;
        else total += v;
    }
    return total;
}
