/*
 * A336356  Characteristic function of A336359, numbers k for which
 *          A001222(A000203(k)) < A001222(k).
 * https://oeis.org/A336356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A001222(A000203(n)) < A001222(n), else 0. bigomega
 *            is additive, so bigomega(sigma(n)) is summed over the factors
 *            (1+p+...+p^e) of sigma(n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8548 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A336356_bo(uint64_t m)
{
    unsigned c = 0;
    while ((m & 1) == 0 && m > 1) { c++; m >>= 1; }
    for (uint64_t i = 3; i <= m / i; i += 2) {
        while (m % i == 0) { c++; m /= i; }
    }
    if (m > 1) c++;
    return c;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned om = 0, os = 0;
    uint64_t p = 2;
    while (p <= m / p) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
                om++;
            }
            os += A336356_bo(term);
        }
        p += (p == 2) ? 1 : 2;
    }
    if (m > 1) {
        om++;
        os += A336356_bo(m + 1);
    }
    return (seq_t)(os < om ? 1 : 0);
}
