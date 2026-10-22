/*
 * A356929  Integers with an even number of even digits.
 * https://oeis.org/A356929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer that has an even number of even digits
   (digits 0,2,4,6,8). The function scans successive integers and
   tests the parity of their even‑digit count using the provided
   digit_counts utility. This is efficient for n up to tens of
   thousands because roughly half of all integers satisfy the
   condition. */

#include <stdbool.h>
#include "../utils/digit_counts.h"

/* Return true if x contains an even number of even digits. */
static bool has_even_even_digits(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    seq_t ev = (seq_t)cnt[0] + (seq_t)cnt[2] + (seq_t)cnt[4]
             + (seq_t)cnt[6] + (seq_t)cnt[8];
    return (ev & 1) == 0;          /* 0 means even parity */
}

seq_t a(idx_t n)
{
    /* n is 1‑based; search from 1 upwards. */
    seq_t val = (seq_t)1;
    while (1) {
        if (has_even_even_digits(val)) {
            if (n == (idx_t)1)
                return val;
            --n;                  /* found one, look for the next */
        }
        val++;                     /* examine next integer */
    }
}
