/*
 * A342465  a(n) = A329697(A337194(n)).
 * https://oeis.org/A342465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329697(A337194(n)), where A337194(n) = 1 +
 *            oddpart(sigma(n)); A329697(m) = 0 if m is a power of 2, else 1 +
 *            A329697(m - m/lpf(m)), lpf = largest prime factor
 * Range:     u8 n <= 989, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9841 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A342465_sigma(idx_t n)
{
    seq_t s = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            seq_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
            }
            s *= term;
        }
    }
    if (m > 1) s *= ((seq_t)m + 1);
    return s;
}

static seq_t A342465_lpf_odd(seq_t m)
{
    seq_t largest = 1;
    for (seq_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    seq_t s = A342465_sigma(n);
    while ((s & 1) == 0) s >>= 1;
    seq_t k = s + 1;
    seq_t count = 0;
    while ((k & (k - 1)) != 0) {
        while ((k & 1) == 0) k >>= 1;
        seq_t p = A342465_lpf_odd(k);
        k = k - k / p;
        count++;
    }
    return count;
}
