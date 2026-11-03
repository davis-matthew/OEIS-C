/*
 * A248210  Zeroless numbers k (numbers in A052382) such that k -
 *          DigitProduct(k) contains the same distinct digits as k.
 * https://oeis.org/A248210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248210:  Zeroless numbers k such that k - digit_product(k)
   contains the same distinct digits as k.  The index n starts at 1. */
#include <stdbool.h>

/* Compute the product of the decimal digits of x.  If x contains a zero
   digit the result is 0. */
static seq_t digit_product_calculate(seq_t x)
{
    seq_t prod = 1;
    while (x > 0) {
        int d = (int)(x % 10);
        prod *= (seq_t)d;
        x /= 10;
    }
    return prod;
}

/* Compare the sets of distinct decimal digits of a and b. */
static bool same_distinct_digits(seq_t a, seq_t b)
{
    unsigned mask_a = 0, mask_b = 0;
    seq_t x = a;
    while (x > 0) {
        int d = (int)(x % 10);
        mask_a |= 1u << d;
        x /= 10;
    }
    x = b;
    while (x > 0) {
        int d = (int)(x % 10);
        mask_b |= 1u << d;
        x /= 10;
    }
    return mask_a == mask_b;
}

/* Return true if the decimal representation of x contains a zero digit. */
static bool contains_zero(seq_t x)
{
    while (x > 0) {
        if ((x % 10) == 0)
            return true;
        x /= 10;
    }
    return false;
}

/* Return the nth member of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;
    while (true) {
        /* skip numbers containing the digit zero */
        if (!contains_zero(k)) {
            seq_t prod = digit_product_calculate(k);
            seq_t diff = k - prod;
            if (same_distinct_digits(k, diff)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++k;
    }
}
