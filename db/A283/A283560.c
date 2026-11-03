/*
 * A283560  Concatenation of the first n nonsquares (A000037).
 * https://oeis.org/A283560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 340 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283560: a(n) = concatenation of the first n non‑squares (A000037) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* integer square root for uint64_t values (floor of sqrt(n)) */
static idx_t isqrt_idx(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t candidate = 2;
    idx_t count = 0;

    while (count < n) {
        idx_t root = isqrt_idx(candidate);
        if (root * root != candidate) {          /* candidate is a non‑square */
            seq_t term = (seq_t)candidate;
            int d = num_digits(term);              /* number of decimal digits */
            result = result * pow10((idx_t)d) + term;
            ++count;
        }
        ++candidate;
    }
    return result;
}
