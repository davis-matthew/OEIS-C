/*
 * A246210  Permutation of nonnegative integers: a(0) = 0, a(1) = 1, and for n
 *          > 1, if A117966(n) < 1, a(n) = 1 + 2*a(-(A117966(n))), otherwise
 *          a(n) = 2*a(A117966(n)-1).
 * https://oeis.org/A246210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, and for n > 1, if A117966(n) < 1, a(n) = 1 +
 *            2*a(-A117966(n)), otherwise a(n) = 2*a(A117966(n)-1); A117966(n)
 *            = n read in base 3 with digit 2 taken as -1
 * Range:     u8 n <= 27, u16 n <= 252, u32 n <= 2187, u64 n <= 2187,
 *            u128 n <= 2187, big any n
 * Validated: standard, pass, 92 terms through n = 2017 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* A117966(n): ternary digits of n, with digit 2 replaced by -1 (no carry) */
static __int128 A117966_signed(idx_t n)
{
    __int128 val = 0, p = 1;
    while (n > 0) {
        int d = (int)(n % 3);
        n /= 3;
        if (d == 1) val += p;
        else if (d == 2) val -= p;
        p *= 3;
    }
    return val;
}

seq_t a(idx_t n)
{
    size_t cap = 64, len = 0;
    unsigned char *bits = (unsigned char *)malloc(cap);
    idx_t cur = n;
    while (cur > 1) {
        __int128 b = A117966_signed(cur);
        unsigned char bit;
        if (b < 1) {
            bit = 1;
            cur = (idx_t)(-b);
        } else {
            bit = 0;
            cur = (idx_t)(b - 1);
        }
        if (len == cap) {
            cap *= 2;
            bits = (unsigned char *)realloc(bits, cap);
        }
        bits[len++] = bit;
    }
    seq_t r = (seq_t)(cur == 1 ? 1 : 0);
    while (len > 0) {
        len--;
        r = r * (seq_t)2 + (seq_t)bits[len];
    }
    free(bits);
    return r;
}
