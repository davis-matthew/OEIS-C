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
 * Helpers:   digit_product_calculate, same_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248210:  Zeroless numbers k such that k - digit_product(k)
   contains the same distinct digits as k.  The index n starts at 1. */
#include <stdbool.h>
#include "../utils/digit_product_calculate.h"
#include "../utils/same_distinct_digits.h"

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
