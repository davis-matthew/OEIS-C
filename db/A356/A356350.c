/*
 * A356350  Primitive terms of A357769: terms of A357769 that are not ten
 *          times another term of A357769.
 * https://oeis.org/A356350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number m in A357769 such that m is not 10*k with k
 *            in A357769; computed by brute-force search. A357769: every
 *            prefix digit-sum of m divides m
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool A357769_valid(seq_t N)
{
    uint8_t digits[128];
    size_t len = 0;
    seq_t t = N;
    if (t == (seq_t)0) return false;
    while (t > (seq_t)0) {
        digits[len++] = (uint8_t)(t % (seq_t)10);
        t = t / (seq_t)10;
    }
    seq_t sum = (seq_t)0;
    for (size_t i = len; i > 0; --i) {
        sum += (seq_t)digits[i - 1];
        if (sum == (seq_t)0) return false;
        if (N % sum != (seq_t)0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    idx_t count = (idx_t)0;
    seq_t c = (seq_t)1;
    while (true) {
        if (A357769_valid(c)) {
            bool prim = true;
            if (c % (seq_t)10 == (seq_t)0 && A357769_valid(c / (seq_t)10)) prim = false;
            if (prim) {
                ++count;
                if (count == n) return c;
            }
        }
        ++c;
    }
}
