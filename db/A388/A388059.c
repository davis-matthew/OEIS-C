/*
 * A388059  Smallest multiple of n that does not contain the digit 1 in its
 *          decimal representation.
 * https://oeis.org/A388059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute smallest multiple of n that does not contain digit 1 (brute force search) */
#include <stdbool.h>

/* helper: true if x has no decimal digit 1 */
static inline bool no_digit_one(seq_t x)
{
    const seq_t ten = (seq_t)10;
    while (x > (seq_t)0) {
        if (x % ten == (seq_t)1)
            return false;
        x /= ten;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    for (idx_t k = 1;; ++k) {
        seq_t mult = base * (seq_t)k;
        if (no_digit_one(mult))
            return mult;
    }
}
