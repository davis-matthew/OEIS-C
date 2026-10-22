/*
 * A321480  Zeroless analog of triangular numbers (version 2): a(0) = 0, and
 *          for any n > 0, a(n) = noz(1 + noz(2 + ... + noz((n-1) + n))),
 *          where noz(n) = A004719(n) omits the zeros from n.
 * https://oeis.org/A321480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = noz(1 + noz(2 + ... + noz((n-1) + n))) (a(0) = 0)
 * Range:     u8 n <= 33, u16 n <= 4087, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Remove all zero digits from the decimal representation of x.
 * Works for all unsigned integer seq_t types, including arbitrarily
 * large ones that support the usual arithmetic operators and division
 * by 10. */
static seq_t remove_zeros(seq_t x) {
    /* Largest expected number of decimal digits for the ranges we
       handle in the sequence is far below 200, but we use a small
       static buffer to avoid dynamic allocation. */
    unsigned digits[200];
    size_t len = 0;
    while (x != (seq_t)0) {
        digits[len++] = (unsigned)(x % (seq_t)10);
        x /= (seq_t)10;
    }
    seq_t res = (seq_t)0;
    for (size_t i = len; i-- > 0; ) {
        unsigned d = digits[i];
        if (d != 0) {
            res = res * (seq_t)10 + (seq_t)d;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Start with the innermost addition (n-1)+n and remove zeros */
    seq_t temp = (seq_t)(n - (idx_t)1) + (seq_t)n;
    temp = remove_zeros(temp);

    /* Now iterate i = n-2 down to 1, adding and removing zeros each step */
    if (n > (idx_t)1) {
        for (idx_t i = n - (idx_t)2; i > (idx_t)0; --i) {
            temp = remove_zeros(temp + (seq_t)i);
        }
    }

    return temp;
}
