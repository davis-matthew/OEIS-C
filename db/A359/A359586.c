/*
 * A359586  Inverse Möbius transform of A359581.
 * https://oeis.org/A359586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} f(p,e), where with c = A329697(p): c
 *            even: f = e+1; c odd: f = 1 if e even, 0 if e odd.
 *            (Multiplicative)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9454 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A359586_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

static uint64_t A359586_steps(uint64_t k)
{
    uint64_t c = 0;
    while (k > 1 && (k & (k - 1)) != 0) {
        uint64_t p = A359586_lpf(k);
        k -= k / p;
        c++;
    }
    return c;
}

static seq_t A359586_term(uint64_t p, uint64_t e)
{
    uint64_t c = A359586_steps(p);
    if ((c & 1ULL) == 0)
        return (seq_t)(e + 1);
    return (seq_t)((e & 1ULL) ? 0 : 1);
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t res = 1;
    if (m == 0) return 0;
    uint64_t e = 0;
    while ((m & 1ULL) == 0) { m >>= 1; e++; }
    if (e) res = res * A359586_term(2, e);
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            e = 0;
            while (m % f == 0) { m /= f; e++; }
            res = res * A359586_term(f, e);
            if (res == 0) return 0;
        }
    }
    if (m > 1) res = res * A359586_term(m, 1);
    return res;
}
