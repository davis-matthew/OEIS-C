/*
 * A275106  Limiting sequence of the least significant digits of the
 *          even-indexed terms of A027878 in reverse order.
 * https://oeis.org/A275106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = digit n-1 (from the right, 0-indexed) of s *
 *            Product_{i=1..n-1}(10^i-1) mod 10^n, where s = (-1)^(n+1);
 *            factors with i>=n are all -1 mod 10^n, so the low digits of
 *            A027878(2m) stabilize. Computed with a decimal digit array,
 *            O(n^2)
 * Range:     u8 n <= 300, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Validated: standard, pass, 96 terms through n = 276 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t len = (size_t)n;
    int *D = (int *)calloc(len, sizeof(int));
    int *T = (int *)calloc(len, sizeof(int));
    D[0] = 1;
    for (size_t i = 1; i + 1 <= len; ++i) {
        /* T = D*10^i - D mod 10^len */
        int borrow = 0;
        for (size_t j = 0; j < len; ++j) {
            int v = (j >= i ? D[j - i] : 0) - D[j] - borrow;
            if (v < 0) { v += 10; borrow = 1; } else borrow = 0;
            T[j] = v;
        }
        int *t = D; D = T; T = t;
    }
    if (len % 2 == 0) {
        /* negate mod 10^len */
        int borrow = 0;
        for (size_t j = 0; j < len; ++j) {
            int v = 0 - D[j] - borrow;
            if (v < 0) { v += 10; borrow = 1; } else borrow = 0;
            T[j] = v;
        }
        int *t = D; D = T; T = t;
    }
    int digit = D[len - 1];
    free(D);
    free(T);
    return (seq_t)digit;
}
