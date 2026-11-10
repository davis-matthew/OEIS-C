/*
 * A376360  Positions of numbers in A007961 that end in 3.
 * https://oeis.org/A376360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position m of the n-th term of A007961 whose last digit
 *            is 3 (search over m, tracking only the last digit of A007961(m))
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t A376360_isqrt(idx_t n) {
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* last digit of A007961(m), using the same greedy digit rule */
static int A376360_last_digit(idx_t m) {
    idx_t k = A376360_isqrt(m);
    idx_t remaining = m;
    idx_t digit = 0;
    while (k >= 1) {
        idx_t square = k * k;
        idx_t max_digit = (k * k + 2 * k) / square;
        digit = remaining / square;
        if (digit > max_digit) digit = max_digit;
        remaining -= digit * square;
        k--;
    }
    return (int)(digit % 10);
}

static idx_t *A376360_cache = 0;
static idx_t A376360_count = 0;
static idx_t A376360_cap = 0;
static idx_t A376360_next = 1;

seq_t a(idx_t n) {
    while (A376360_count < n) {
        idx_t m = A376360_next++;
        if (A376360_last_digit(m) == 3) {
            if (A376360_count == A376360_cap) {
                idx_t nc = A376360_cap ? A376360_cap * 2 : 1024;
                A376360_cache = (idx_t *)realloc(A376360_cache, (size_t)nc * sizeof(idx_t));
                A376360_cap = nc;
            }
            A376360_cache[A376360_count++] = m;
        }
    }
    return (seq_t)A376360_cache[n - 1];
}
