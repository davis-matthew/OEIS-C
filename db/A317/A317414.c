/*
 * A317414  Continued fraction for ternary expansion of Liouville's number
 *          interpreted in base 3 (A012245).
 * https://oeis.org/A317414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0)=0; for n>=1: a(n)=3^((m-1)*m!)-1 if n+1=2^m*u with u==1
 *            (mod 4), m>1, or n=2^m*u with u==3 (mod 4), m>1; else a(n)=3 if
 *            n mod 16 in {5,10}; 4 if n mod 16 in {2,13}; 2 if
 *            n=8k-10+3*A089013(k-1) for some k>0; otherwise 1
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 14, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 60 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A317414_pow3(uint64_t e) {
    seq_t r = 1, b = 3;
    while (e) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

static int A317414_a089013(idx_t n) {
    if (n == 0) return 1;
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;
    return (int)(((m - 1) >> 1) & 1);
}

static seq_t A317414_big(idx_t m) {
    uint64_t f = 1;
    for (idx_t i = 2; i <= m; i++) f *= (uint64_t)i;
    return A317414_pow3((uint64_t)(m - 1) * f) - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    idx_t x = n + 1;
    idx_t m = 0;
    while ((x & 1) == 0) { x >>= 1; m++; }
    if (m > 1 && (x & 3) == 1) return A317414_big(m);

    x = n;
    m = 0;
    while ((x & 1) == 0) { x >>= 1; m++; }
    if (m > 1 && (x & 3) == 3) return A317414_big(m);

    idx_t r16 = n & 15;
    if (r16 == 5 || r16 == 10) return (seq_t)3;
    if (r16 == 2 || r16 == 13) return (seq_t)4;

    idx_t r8 = n & 7;
    if (r8 == 6) {
        idx_t k = (n + 10) / 8;
        if (A317414_a089013(k - 1) == 0) return (seq_t)2;
    } else if (r8 == 1) {
        idx_t k = (n + 7) / 8;
        if (A317414_a089013(k - 1) == 1) return (seq_t)2;
    }
    return (seq_t)1;
}
