/*
 * A285660  Degree of the algebraic number sin(n degrees) = sin(n Pi/180
 *          radians).
 * https://oeis.org/A285660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(m) where m = 180/gcd(n,180); if m ≡ 2 (mod 4) and m ≠
 *            2, then a(n)=φ(m)/2; if m = 2 then a(n)=1. This gives the degree
 *            of the minimal polynomial of sin(n°)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned _phi_div180(uint64_t m) {
    unsigned result = (unsigned)m;
    if (m % 2 == 0) {
        result -= result / 2;
        while (m % 2 == 0) m /= 2;
    }
    if (m % 3 == 0) {
        result -= result / 3;
        while (m % 3 == 0) m /= 3;
    }
    if (m % 5 == 0) {
        result -= result / 5;
        while (m % 5 == 0) m /= 5;
    }
    return result;
}

static inline idx_t _gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t d = _gcd_idx(n, (idx_t)180);
    uint64_t m = 180ULL / d;
    unsigned phi = _phi_div180(m);

    unsigned deg;
    if (m == 2ULL)
        deg = 1U;
    else if (m % 4ULL == 2ULL)
        deg = phi / 2U;
    else
        deg = phi;   /* m odd or divisible by 4 */

    return (seq_t)deg;
}
