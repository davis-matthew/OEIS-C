/*
 * A349339  Odd bisection of the Möbius transform of A126760.
 * https://oeis.org/A349339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A347233(2*n-1), where A347233(m) = Sum_{d|m}
 *            mu(m/d)*A126760(d) (Moebius transform of A126760)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A126760_a(idx_t n)
{
    seq_t result = 0;
    idx_t m = n;

    while (m > 0) {
        idx_t r = m % 6;

        if (r == 1) {
            result = ((seq_t)2 * ((seq_t)(m - 1) / 6)) + 1;
            break;
        }
        if (r == 5) {
            result = ((seq_t)2 * ((seq_t)(m - 5) / 6)) + 2;
            break;
        }
        if ((m & 1) == 0) {
            m >>= 1;
            continue;
        }
        if (m % 3 == 0) {
            m /= 3;
            continue;
        }
        break;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n - 1;
    idx_t ps[24];
    int k = 0;
    idx_t t = m;
    idx_t p;

    for (p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            ps[k++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) ps[k++] = t;

    seq_t pos = 0, neg = 0;
    unsigned long mask;
    for (mask = 0; mask < (1UL << k); mask++) {
        idx_t d = m;
        int bits = 0;
        int i;
        for (i = 0; i < k; i++) {
            if (mask & (1UL << i)) {
                d /= ps[i];
                bits++;
            }
        }
        if (bits & 1) neg += A126760_a(d);
        else pos += A126760_a(d);
    }
    return pos - neg;
}
