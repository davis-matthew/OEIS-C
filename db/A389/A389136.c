/*
 * A389136  a(n) = 2*sqrt(n/A037449(n)).
 * https://oeis.org/A389136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   write n = s*k^2 with s squarefree; a(n) = 2*k if s == 1 (mod 4),
 *            else k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9948 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t sf = 1, k = 1;
    idx_t p;

    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        if (e & 1) sf *= 2;
        for (int i = 0; i < e / 2; ++i) k *= 2;
    }
    for (p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e & 1) sf *= p;
            for (int i = 0; i < e / 2; ++i) k *= p;
        }
    }
    if (tmp > 1) sf *= tmp;

    if (sf % 4 == 1)
        return (seq_t)k * 2;
    return (seq_t)k;
}
