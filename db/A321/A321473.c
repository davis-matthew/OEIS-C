/*
 * A321473  Nonnegative numbers whose nonzero digits in ternary expansion are
 *          palindromic.
 * https://oeis.org/A321473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate numbers, returning the n‑th nonnegative integer whose
   nonzero digits in base‑3 form a palindrome.  This implements
   sequence A321473. */

#include <stdbool.h>
#include <stddef.h>

/* Helper: true iff the nonzero ternary digits of x are a palindrome. */
static bool is_good(seq_t x)
{
    /* Holds the nonzero ternary digits in reverse order. */
    unsigned char d[128];
    size_t count = 0;

    while (x > 0) {
        seq_t digit = x % 3;
        if (digit != 0) {
            d[count++] = (unsigned char)digit;
        }
        x /= 3;
    }

    /* Empty list (x == 0) is considered a palindrome. */
    for (size_t l = 0, r = count; l < r; ++l, --r) {
        if (d[l] != d[r - 1]) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1, so n >= 1.  The function works for any n >= 1. */
    seq_t cur = 0;
    idx_t found = 0;

    while (found < n) {
        if (is_good(cur)) {
            ++found;
            if (found == n) {
                break;
            }
        }
        ++cur;
    }
    return cur;
}
